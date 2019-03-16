#ifndef SERVER_HPP
#define SERVER_HPP

#include <uv.h>
#include <iostream>
#include <queue>

#include "Parser.hpp"

#define CONNECTIONS_COUNT  128

struct Query {
    std::queue<uv_buf_t> queue;
    uv_mutex_t mutex;
};

struct Worker {
    unsigned short int id;
    uv_thread_t thread;
    uv_loop_t *loop;
    Query *queries;
};

class Server {
    public:
        Server(const std::string, const unsigned short, const std::string, const unsigned short);
        ~Server();
    private:
        unsigned short threadsCount;
        uv_tcp_t server;	// сокет
        uv_loop_t *loop;
        uv_mutex_t mutex;

        Query queries;
        std::vector<Worker*> workers;
        
        // std::string root;

        // void threadCB();
};

#endif  // SERVER_HPP