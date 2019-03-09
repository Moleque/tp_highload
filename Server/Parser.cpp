#include "Parser.hpp"

void get_uri_without_parameters(char* src, char* dest, int max) {
    char* ptr = strchr(src, '?');
    if (ptr) {
        max = ptr - src;
    }
    strncpy(dest, src, max);
    dest[max] = '\0';
}

void decode_uri(char* src, char* dest, int max) {
    char *p = src;
    char code[3] = {0};
    while (*p != '\0' && --max) {
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

int parseHttp(const char *buffer) {
    // printf("%s", buffer);
        
    // struct evbuffer *event_buffer = client_buffer;
    // struct stat sbuf;     /* file status */
    // size_t line_size = LINE_BUFSIZE;


    /* get the HTTP request line */
    // if(strcpy(http_request->buf, evbuffer_readln(event_buffer, &line_size, EVBUFFER_EOL_CRLF)) == NULL) {
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


    // /* parse the uri */
    if (!strstr(request.uri, "cgi-bin")) {
        strcpy(request.cgiargs, "");
        strcpy(request.filename, root_dir);
        if(strstr(request.uri, "%")) {
            // printf("uri decoding\n");
            decode_uri(request.uri, request.uri, strlen(request.uri));
            printf("URI = %s\n", request.uri);
        }
        if(strstr(request.uri, "?")) {
            printf("uri need to be reed out of parametres\n");
            char temp[strlen(request.uri)];
            get_uri_without_parameters(request.uri, temp, strlen(request.uri));
            strcpy(request.uri, temp);
            printf("URI = %s\n", request.uri);

        }

        strcat(request.filename, request.uri);
        if (request.uri[strlen(request.uri)-1] == '/') {
            strcat(request.filename, "index.html");
            if (stat(request.filename, &sbuf) < 0) {
                return INDEX_FILE_NOT_EXIST;
            }
        }
    }

    // printf("%s\n", http_request->filename);

    // if ((strstr(http_request->filename, "/..")) != NULL) {
    //     return ESCAPING_ROOT;
    // }

    // /* make sure the file exists */
    // if (stat(http_request->filename, &sbuf) < 0) {
    //     return FILE_NOT_EXIST;
    // }

    // /* serve static content */
    // if (strstr(http_request->filename, ".html"))
    //     strcpy(http_request->filetype, "text/html");
    // else if (strstr(http_request->filename, ".css"))
    //     strcpy(http_request->filetype, "text/css");
    // else if (strstr(http_request->filename, ".js"))
    //     strcpy(http_request->filetype, "application/javascript");
    // else if (strstr(http_request->filename, ".gif"))
    //     strcpy(http_request->filetype, "image/gif");
    // else if (strstr(http_request->filename, ".swf"))
    //     strcpy(http_request->filetype, "application/x-shockwave-flash");
    // else if (strstr(http_request->filename, ".jpeg"))
    //     strcpy(http_request->filetype, "image/jpeg");
    // else if (strstr(http_request->filename, ".jpg"))
    //     strcpy(http_request->filetype, "image/jpeg");
    // else if (strstr(http_request->filename, ".png"))
    //     strcpy(http_request->filetype, "image/png");

    // http_request->filesize = (size_t) sbuf.st_size;
    // if (!(S_ISREG(sbuf.st_mode))) {
    //     return FILE_IS_EXECUTABLE;
    // }

    // return ALL_OK;

    return 0;
}