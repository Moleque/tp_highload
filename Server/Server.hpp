#ifndef SERVER_HPP
#define SERVER_HPP

#include <uv.h>
#include <iostream>
#include <queue>

#include "Parser.hpp"

#define CONNECTIONS_COUNT  1024

class Server {
    public:
        Server(const std::string, const unsigned short, const std::string, const unsigned short);
        ~Server();
    private:
        std::string rootDir;

        unsigned short threadsCount;
        std::vector<uv_work_t*> workers;
};

#endif  // SERVER_HPP