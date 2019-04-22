#ifndef SERVER_HPP
#define SERVER_HPP

#include <uv.h>
#include <iostream>
#include <queue>

#include "Parser.hpp"

#define CONNECTIONS_COUNT  1024



struct Query {
    char *data = nullptr;
    size_t lenght = 0;
    uv_stream_t *client;
};
    
struct ThreadStorage {
    std::queue<Query*> queries;
    uv_mutex_t *mutex;
};

// struct Worker {
//     unsigned short int id;
//     uv_thread_t thread;
//     uv_loop_t *loop;
//     Query *queries;
// };

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