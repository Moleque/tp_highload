#include "iostream"
#include "./Server/Server.hpp"

#define PORT 7373
#define THREADS 2

int main(int argc, char **argv) {
    Server *server = new Server("0.0.0.0", PORT);
    std::cout << "Hi" << std::endl;

    int v;
    std::cin >> v;
    return 0;
}