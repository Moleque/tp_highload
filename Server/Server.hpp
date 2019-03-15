#ifndef SERVER_HPP
#define SERVER_HPP

#include <uv.h>
#include <iostream>

#include "Parser.hpp"

#define CONNECTIONS_COUNT  128

struct Worker {
    unsigned short int id;
    bool flag = false;
    uv_thread_t thread;
    uv_loop_t *loop;
};

class Server {
    public:
        Server(const std::string, const unsigned short, const std::string, const unsigned short);
    private:
        unsigned short threadsCount;
        uv_tcp_t server;	// сокет
        // std::string root;

        // void threadCB();
};

#endif  // SERVER_HPP