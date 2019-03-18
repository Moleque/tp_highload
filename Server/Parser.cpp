#include "Parser.hpp"

Http::Http(const std::string request, const std::string root) {
    this->request.data = request;
    this->request.filename = root;
}

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
    // if (!request.uri.find("cgi-bin") != std::string::npos) {
        // strcpy(cgiargs, "");

        if (request.uri.find("%") != std::string::npos) {
            // printf("request.uri decoding\n");
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
    // }

    if (request.filename.find("/..") != std::string::npos) {
        // response.length = 0;
        return FORBIDDEN;
    }
    
    // std::cout << "File: " << request.filename;
    // проверка существования файла
    struct stat fileStat;
    if (stat(request.filename.c_str(), &fileStat) < 0) {
        // response.length = 0;
        return NOT_FOUND;
    }

    // определение mime-типа
    for (auto type : mimeTypes) {
        if (request.filename.find(type.format) != std::string::npos) {
            response.mimetype = type.mime;
        }
    }
    
    


    response.length = (size_t)fileStat.st_size;
    // if (!(S_ISREG(sbuf.st_mode))) {
    //     return FILE_IS_EXECUTABLE;
    // }

    return OK;
}

void Http::parseUri(char *src, char *dest, int len) {
    char *p = src;
    char code[3] = {0};
    while (*p != '\0'){// && --len) {
        if (*p == '%') {
            memcpy(code, ++p, 2);
            *dest++ = (char) strtoul(code, NULL, 16);
            p += 2;
        } else {
            *dest++ = *p++;
        }
    }
    *dest = '\0';
}

std::string Http::parseTime(const time_t time) {
    char buf[80];
    struct tm tm = *gmtime(&time);
    strftime(buf, sizeof(buf), "%a, %d, %b, %Y %H:%M:%S %Z", &tm);
    return std::string(buf);
}

bool Http::parseFile(const std::string filename, char *buffer, const size_t length) {
    // std::string fileContent;
    
    int file = open(filename.c_str(), O_RDONLY);
    if (file < 0) {
        return false;
    }

    char *map = (char*)mmap(0, length, PROT_READ, MAP_SHARED, file, 0);
    if (map == MAP_FAILED) {
        close(file);
        return false;
    }

    memcpy(buffer + response.data.size(), map, length);

    if (munmap(map, length) == -1) {
        close(file);
        return false;
    }
    close(file);
    return true; 


    // std::ifstream in(filename, std::ios::binary);
    // char *buffer = new char[length];
    // in.read(buffer, length);
    // response.data += buffer;
    // // std::cout <<
    // in.close();
    // delete buffer;
    // return "";
}

size_t Http::getResponse(char *buffer) {
    if (response.data.empty()) {
        int status = parseHttp();
        response.status = std::to_string(status);
        response.phrase = statusPhrase.at(status);

        response.data = "HTTP/1.1 " + response.status + " " + response.phrase + "\r\n";
        response.data += "Server: MLQ/0.1.2\r\n";
        response.data += "Connection: close\r\n";
        
        time_t now = time(NULL);
        response.date = parseTime(now);
        response.data += "Date: " + response.date + "\r\n";

        // if (response.status == std::to_string(OK)) {
            response.data += "Content-Length: " + std::to_string(response.length) + "\r\n";
            response.data += "Content-Type: " + response.mimetype + "\r\n";


            // if (request.method != "HEAD") {
                response.data += "\r\n";
                response.size = response.data.size() + response.length;

                // buffer = (char*)malloc(response.size);
                strcpy(buffer, response.data.c_str());
                response.data += parseFile(request.filename, buffer, response.length);
            // } else {
            // response.data += "Content-Type: " + response.mimetype + "\0";
        // }
        //     std::string errMessage = "<html><head><body>Error</body></head></html>\0";
        //     response.data += "Content-Type: " + std::to_string(errMessage.size()) + "\r\n";
        //     response.data += "Content-Type: text/html\r\n";
        //     response.data += "\r\n";
        //     response.data += errMessage;            
        // }
        // const int size = response.data.length();
        // if (response.data) {
        //     response.data += res
        // }
    }
    // std::cout << buffer << std::endl;
    return response.size;
}