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
    size_t size = 0;
};

// структура запроса к серверу
class Http {
    public:
        Http(const std::string, const std::string);
        size_t getResponse(char*);
        std::string getFileContent(const std::string, const size_t);
    private:
        Request request;
        Response response;

        int parseHttp();
        void parseUri(char*, char*, int);
        bool parseFile(const std::string, char*, const size_t);
        std::string parseTime(const time_t);
};

#endif  // PARSER_HPP
