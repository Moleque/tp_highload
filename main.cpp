#include "iostream"
#include "./Settings/Config.hpp"
#include "./Server/Server.hpp"

#define IP "0.0.0.0"
#define PORT 80

#define CFG_PATH "/etc/httpd.conf"


int main(int argc, char **argv) {
    Settings config;
    if (parseConfig(CFG_PATH, &config)) {
        std::cerr << "config file not found\nset default setting" << std::endl;
    }

    Server *server = new Server(IP, PORT, config.root, config.cpu);
    return 0;
}