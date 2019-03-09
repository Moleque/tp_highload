#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdio.h>
#include <string.h>

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

#define RESPONSE    1024
#define REQUEST     1024

// структура запроса к серверу
struct Http {
    char method[REQUEST];
    char uri[REQUEST];
    char version[REQUEST];
};


int parseHttp(const char*);

#endif  // PARSER_HPP
