#include "iostream"
#include "./Settings/Config.hpp"
#include "./Server/Server.hpp"

#define PORT 8080
#define THREADS 2

#define CFG_PATH "./httpd.conf"


int main(int argc, char **argv) {
    Settings config;
    if (parseConfig(CFG_PATH, &config)) {
        std::cerr << "config file not found\nset default setting" << std::endl;
    }

    Server *server = new Server("0.0.0.0", PORT);
    std::cout << "Hi" << std::endl;

    int v;
    std::cin >> v;
    return 0;
}