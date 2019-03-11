#include "Parser.hpp"

void get_uri_without_parameters(char *src, char *dest, int len) {
    char *ptr = strchr(src, '?');
    if (ptr) {
        len = ptr - src;
    }
    strncpy(dest, src, len);
    dest[len] = '\0';
}

void decode_uri(char *src, char *dest, int len) {
    char *p = src;
    char code[3] = {0};
    while (*p != '\0' && --len) {
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

int parseHttp(const char *buffer) {
    // printf("%s", buffer);
        
    // struct evbuffer *event_buffer = client_buffer;
    // struct stat sbuf;     /* file status */
    // size_t line_size = LINE_BUFSIZE;


    /* get the HTTP request line */
    // if(strcpy(request.buf, evbuffer_readln(event_buffer, &line_size, EVBUFFER_EOL_CRLF)) == NULL) {
    //     return PARSE_ERROR;
    // }


    struct Http request;

    // получение метода, uri и версию протокола
    if ((sscanf(buffer, "%s %s %s\n", request.method, request.uri, request.version)) < 3) {
        return -1;
    }

    // проверка метода на валидность
    if (strcmp(request.method, "GET") && strcmp(request.method, "HEAD")) {
        return METHOD_NOT_ALLOWED;
    }


    // парсер uri
    if (!strstr(request.uri, "cgi-bin")) {
        // strcpy(request.cgiargs, "");
        strcpy(request.filename, "");//root_dir);

        if (strstr(request.uri, "%")) {
            // printf("uri decoding\n");
            decode_uri(request.uri, request.uri, strlen(request.uri));
            printf("URI = %s\n", request.uri);
        }
        if (strstr(request.uri, "?")) {
            printf("uri need to be reed out of parametres\n");
            char temp[strlen(request.uri)];
            get_uri_without_parameters(request.uri, temp, strlen(request.uri));
            strcpy(request.uri, temp);
            printf("URI = %s\n", request.uri);
        }

        strcat(request.filename, request.uri);
        if (request.uri[strlen(request.uri)-1] == '/') {
            strcat(request.filename, "index.html");
            // if (stat(request.filename, &sbuf) < 0) {
            //     return INDEX_FILE_NOT_EXIST;
            // }
        }
    }

    printf("%s\n", request.filename);

    // if ((strstr(request.filename, "/..")) != NULL) {
    //     return ESCAPING_ROOT;
    // }
    // /* make sure the file exists */
    // if (stat(request.filename, &sbuf) < 0) {
    //     return FILE_NOT_EXIST;
    // }

    /* serve static content */
    if (strstr(request.filename, ".html")) {
        strcpy(request.mimetype, "text/html");
    } else if (strstr(request.filename, ".css")) {
        strcpy(request.mimetype, "text/css");
    } else if (strstr(request.filename, ".js")) {
        strcpy(request.mimetype, "application/javascript");
    } else if (strstr(request.filename, ".jpg")) {
        strcpy(request.mimetype, "image/jpeg");
    } else if (strstr(request.filename, ".jpeg")) {
        strcpy(request.mimetype, "image/jpeg");
    } else if (strstr(request.filename, ".png")) {
        strcpy(request.mimetype, "image/png");
    } else if (strstr(request.filename, ".gif")) {
        strcpy(request.mimetype, "image/gif");
    } else if (strstr(request.filename, ".swf")) {
        strcpy(request.mimetype, "application/x-shockwave-flash");
    }

    // request.filesize = (size_t) sbuf.st_size;
    // if (!(S_ISREG(sbuf.st_mode))) {
    //     return FILE_IS_EXECUTABLE;
    // }

    return 0;//ALL_OK;
}