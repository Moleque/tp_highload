#ifndef SERVER_HPP
#define SERVER_HPP

// #include <iostream>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

// #include <unistd.h>
// #include <fcntl.h>
// #include <cstring>

// #include <set>
// #include <algorithm>

// #include <sys/epoll.h>

#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

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
        // void newConnectionCB(uv_stream_t*, int);
        int sockfd;

        uv_tcp_t server;	// сокет
        // uv_loop_t *loop;
    //     WSAData wData;
};

#endif  // SERVER_HPP