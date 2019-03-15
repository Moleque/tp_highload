#ifndef SERVER_HPP
#define SERVER_HPP

#include <uv.h>
#include <iostream>

#include "Parser.hpp"

#define CONNECTIONS_COUNT  128

class Server {
    public:
        Server(const std::string, const unsigned short, const std::string);
        ~Server();
    private:
        uv_tcp_t server;	// сокет
        // std::vector<thread> threads;
        // std::string root;
};

#endif  // SERVER_HPP