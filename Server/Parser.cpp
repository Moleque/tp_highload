#include "Parser.hpp"

Http::Http(const std::string request, const std::string root) {
    this->request.data = request;
    this->request.filename = root;
}

void Http::parseUri(char *src, char *dest, int len) {
    char *p = src;
    char code[3] = {0};
    while (*p != '\0') {
        if (*p == '%') {
            memcpy(code, ++p, 2);
            *dest++ = (char) strtoul(code, NULL, 16);
            p += 2;
        } 
        else {
            *dest++ = *p++;
        }
    }
    *dest = '\0';
}

// парсинг HTTP запроса
int Http::parseHttp() {
    // получение метода, uri и версии протокола
    std::istringstream stream(request.data);
    stream >> request.method >> request.uri >> request.version;
    if (request.method.empty() || request.uri.empty() || request.version.empty()) {
        return METHOD_NOT_ALLOWED;
    }

    // проверка метода на валидность
    if (request.method != "GET" && request.method != "HEAD") {
        return METHOD_NOT_ALLOWED;
    }


    // парсер request.uri
    if (request.uri.find("%") != std::string::npos) {
        char ss[request.uri.length()];
        strcpy(ss, request.uri.c_str());
        char tmp[500];
        parseUri(ss, tmp, request.uri.length());
        request.uri = tmp;
    }

    request.uri = request.uri.substr(0, request.uri.find('?')); // получить uri без параметров

    request.filename += request.uri;
    if (request.uri[request.uri.length()-1] == '/') {
        if (request.uri.find(".") == std::string::npos) {
            request.filename += "index.html";
            struct stat isExist;
            if (stat(request.filename.c_str(), &isExist) < 0) {
                return FORBIDDEN;
            }
        }
    }

    if (request.filename.find("/..") != std::string::npos) {
        return FORBIDDEN;
    }
    
    // проверка существования файла
    struct stat fileStat;
    if (stat(request.filename.c_str(), &fileStat) < 0) {
        return NOT_FOUND;
    }

    // определение mime-типа
    for (auto type : mimeTypes) {
        if (request.filename.find(type.format) != std::string::npos) {
            response.mimetype = type.mime;
        }
    }

    response.fileLength = (size_t)fileStat.st_size;
    return OK;
}

// получить текущие дату и время в формате HTTP
std::string Http::parseTime(const time_t time) {
    char buf[80];
    struct tm tm = *gmtime(&time);
    strftime(buf, sizeof(buf), "%a, %d, %b, %Y %H:%M:%S %Z", &tm);
    return std::string(buf);
}

// отправить данные из файла
bool Http::sendFile(int fd, const std::string filename, const size_t length) {
    int file = open(filename.c_str(), O_RDONLY);
    if (file < 0) {
        return false;
    }

    int sentBytes = 0, remainData = length;
    off_t offset = 0;
    while ((sentBytes = sendfile(fd, file, &offset, BUF_SIZE)) > 0) {
        remainData -= sentBytes;
    }
    if (remainData != 0) {
        return false;
    }
    close(file);
    return true;
}

bool Http::sendResponse(int socket) {
    if (response.data.empty()) {
        int status = parseHttp();
        response.status = std::to_string(status);
        response.phrase = statusPhrase.at(status);

        response.data = "HTTP/1.1 " + response.status + " " + response.phrase + "\r\n";
        response.data += "Server: MLQ/0.1.2\r\n";
        response.data += "Connection: close\r\n";

        response.date = parseTime(time(NULL));
        response.data += "Date: " + response.date + "\r\n";

        if (response.status != std::to_string(NOT_FOUND)) {
            response.data += "Content-Length: " + std::to_string(response.fileLength) + "\r\n";
            response.data += "Content-Type: " + response.mimetype + "\r\n";
        }
        
        response.data += "\r\n";
    }

    response.size = response.data.length();
    response.size += request.method == "GET" ? response.fileLength : 0;

    if (send(socket, response.data.c_str(), response.data.length(), 0) < 0) {
        return false;
    }
    if (request.method == "GET" && response.status == std::to_string(OK)) {  // если метод = GET, нужно отправить файл
        if (!sendFile(socket, request.filename, response.fileLength)) {
            return false;
        }
    }
    
    return true;
}