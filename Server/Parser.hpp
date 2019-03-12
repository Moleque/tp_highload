#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <fstream>

#include <stdio.h>
#include <string.h>
#include <cstdlib>

#include "status.hpp"

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

struct Request {
    std::string data;
    std::string method;
    std::string uri;
    std::string version;
    std::string filename;
    std::string mimetype;
};

struct Response {
    std::string data;
};

// структура запроса к серверу
class Http {
    public:
        Http(const std::string);
        std::string getResponse();
    private:
        Request request;
        Response response;

        // std::string request;
        // std::string response;
        int parseHttp();
};

#endif  // PARSER_HPP
