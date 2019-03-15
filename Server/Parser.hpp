#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstdlib>
#include <unistd.h>

#include "Status.hpp"

// typedef struct http {
//     char method[REQUEST_BUFSIZE];  /* request method */
//     char uri[REQUEST_BUFSIZE];     /* request uri */
//     char version[REQUEST_BUFSIZE]; /* request method */
//     char filename[REQUEST_BUFSIZE];/* path derived from uri */
//     char filetype[REQUEST_BUFSIZE];/* path derived from uri */
//     char cgiargs[REQUEST_BUFSIZE]; /* cgi argument list */
//     char buf[REQUEST_BUFSIZE];     /* temporary buffer */
//     size_t filesize;
// } http_t;

struct types {
    const std::string format;
    const std::string mime;
};

const std::vector<types> mimeTypes = {
    { ".html", "text/html" },
    { ".css", "text/css" },
    { ".js", "application/javascript" },
    { ".jpg", "image/jpeg" },
    { ".jpeg", "image/jpeg" },
    { ".png", "image/png" },
    { ".gif", "image/gif" },
    { ".swf", "application/x-shockwave-flash" },
};


struct Request {
    std::string data;
    std::string method;
    std::string uri;
    std::string version;
    std::string filename;
};

struct Response {
    std::string data;
    std::string status;
    std::string phrase;
    std::string date;
    std::string mimetype;
    size_t length = 0;
};

// структура запроса к серверу
class Http {
    public:
        Http(const std::string, const std::string);
        std::string getResponse();
    private:
        Request request;
        Response response;

        int parseHttp();
        void parseUri(char*, char*, int);
        std::string parseFile(const std::string, const size_t);
        std::string parseTime(const time_t);
};

#endif  // PARSER_HPP
