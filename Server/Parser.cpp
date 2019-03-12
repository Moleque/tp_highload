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
    // char test[] = "/sadfdsf/asd/adsfdasf.js?ksadkfj%hsajfhjsdfh%fjshajhfsadhjhfs%fhajhsadjf";
    // char temp[strlen(test)];
    // decode_uri(test, temp, strlen(test));
    // std::cout << temp << std::endl;

    // получение метода, request.uri и версию протокола
    // if ((sscanf(request.data.c_str(), "%s %s %s\n", request.method, request.uri, request.version)) < 3) {
    //     return -1;
    // }

    std::istringstream stream(request.data);
    stream >> request.method >> request.uri >> request.version;

    // проверка метода на валидность
    if (request.method != "GET" && request.method != "HEAD") {
        return METHOD_NOT_ALLOWED;
    }


    // парсер request.uri
    if (!request.uri.find("cgi-bin")) {
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
    }

    std::cout << request.filename << std::endl;

    if (request.filename.find("/..")) {
        return -1;
    }

    // /* make sure the file exists */
    // if (stat(filename, &sbuf) < 0) {
    //     return FILE_NOT_EXIST;
    // }

    /* serve static content */
    if (request.filename.find(".html")) {
        request.mimetype = "text/html";
    } else if (request.filename.find(".css")) {
        request.mimetype = "text/css";
    } else if (request.filename.find(".js")) {
        request.mimetype = "application/javascript";
    } else if (request.filename.find(".jpg")) {
        request.mimetype = "image/jpeg";
    } else if (request.filename.find(".jpeg")) {
        request.mimetype = "image/jpeg";
    } else if (request.filename.find(".png")) {
        request.mimetype = "image/png";
    } else if (request.filename.find(".gif")) {
        request.mimetype = "image/gif";
    } else if (request.filename.find(".swf")) {
        request.mimetype = "application/x-shockwave-flash";
    }

    std::cout << request.mimetype << std::endl;

    // filesize = (size_t) sbuf.st_size;
    // if (!(S_ISREG(sbuf.st_mode))) {
    //     return FILE_IS_EXECUTABLE;
    // }

    return 0;//ALL_OK;
}

// std::string Http::getResponse() {
//     parseHttp();
//     // return "sdf";
//     response = "HTTP/1.1 " + response.status + " " + response.status_msg + "\r\n";
//     response += "Server: HLserver\r\n";
//     response += "Connection: close\r\n";
//     char buf[TIME_BUFSIZE];
//     time_t now = time(NULL);
//     struct tm tm = *gmtime(&now);
//     strftime(buf, sizeof(buf), "%a, %d, %b, %Y %H:%M:%S %Z", &tm);
//     response += "Date: %s\r\n";
// }