#ifndef SERVER_HPP
#define SERVER_HPP

#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

#include "Parser.hpp"

#define CONNECTIONS_COUNT  128

class Server {
    public:
        Server(const char*, const unsigned short);
        ~Server();
        void start();
        void closeS();
        void handle();
        // unsigned short port;
    private:
        // void allocBufferCB(uv_handle_t*, size_t, uv_buf_t*);
        // void readCB(uv_stream_t*, ssize_t, const uv_buf_t*);
        // void socketWriteCB(uv_write_t*, int);
        // void newConnectionCB(uv_stream_t*, int);
        // int sockfd;

        uv_tcp_t server;	// сокет
        // uv_loop_t *loop;
};

#endif  // SERVER_HPP