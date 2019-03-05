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

#include <stdio.h>
#include <stdlib.h>
#include <uv.h>


#define MAX_EV  2048

class Server {
    public:
        Server(const unsigned short);
        ~Server();
        void start();
        void closeS();
        void handle();
        // unsigned short port;
    private:
        int sockfd;
    //     WSAData wData;
};

#endif  // SERVER_HPP