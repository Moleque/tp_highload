#include "iostream"
#include "./Server/Server.hpp"

#define PORT 7373
#define THREADS 2

int main(int argc, char **argv) {
    Server *server = new Server(7373);
    std::cout << "Hi" << std::endl;
    return 0;
}