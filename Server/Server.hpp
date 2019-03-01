#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

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