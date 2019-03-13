#include "Parser.hpp"

Http::Http(const std::string request) {
    this->request.data = request;
}

void decode_uri(char *src, char *dest, int len) {
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


void parserURI() {

}

char parserMIME() {

}

int Http::parseHttp() {
    // получение метода, uri и версии протокола
    std::istringstream stream(request.data);
    stream >> request.method >> request.uri >> request.version;
    if (request.method.empty() || request.uri.empty() || request.version.empty()) {
        return -1;
    }

    // проверка метода на валидность
    if (request.method != "GET" && request.method != "HEAD") {
        return METHOD_NOT_ALLOWED;
    }


    // парсер request.uri
    // if (!request.uri.find("cgi-bin") != std::string::npos) {
        // strcpy(cgiargs, "");
        request.filename = "";//root_dir);

        // if (strstr(request.uri.c_str(), "%")) {
        //     // printf("request.uri decoding\n");
        //     char ss[request.uri.length()] = request.uri.c_str();
        //     char tmp[500];
        //     decode_uri(ss, tmp, request.uri.length());
        //     // printf("URI = %s\n", request.uri);
        // }

        request.uri = request.uri.substr(0, request.uri.find('?')); // получить uri без параметров
        std::cout << "URI = " << request.uri << std::endl;

        request.filename += request.uri;
        if (request.uri[request.uri.length()-1] == '/') {
            request.filename += "index.html";
        }
    // }

    std::cout << "File = " << request.filename << std::endl;

    if (request.filename.find("/..") != std::string::npos) {
        return -1;
    }
    // /* make sure the file exists */
    // if (stat(filename, &sbuf) < 0) {
    //     return FILE_NOT_EXIST;
    // }

    // определение mime-типа
    for (auto type : mimeTypes) {
        if (request.filename.find(type.format) != std::string::npos) {
            std::cout << type.format << std::endl;
            request.mimetype = type.mime;
        }
    }

    

    std::cout << "Mime = " << request.mimetype << std::endl;

    // filesize = (size_t) sbuf.st_size;
    // if (!(S_ISREG(sbuf.st_mode))) {
    //     return FILE_IS_EXECUTABLE;
    // }

    return OK;
}

std::string Http::parseTime(const time_t time) {
    char buf[80];
    struct tm tm = *gmtime(&time);
    strftime(buf, sizeof(buf), "%a, %d, %b, %Y %H:%M:%S %Z", &tm);
    return std::string(buf);
}

std::string Http::getResponse() {
    if (response.data.empty()) {
        response.status = std::to_string(parseHttp());
        response.phrase = "OK";
        response.data = "HTTP/1.1 " + response.status + " " + response.phrase + "\r\n";
        response.data += "Server: MLQ/0.1.2\r\n";
        response.data += "Connection: close\r\n";
        
        time_t now = time(NULL);
        response.date = parseTime(now);
        response.data += "Date: " + response.date + "\r\n";

        if (response.status == std::to_string(OK)) {
            response.data += "Content-Length: " + response.length + "\r\n";
            response.data += "Content-Type: " + request.mimetype + "\r\n";
        }
    }
    return response.data;
}