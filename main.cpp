#include "iostream"
#include <cstdlib>
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

    uv_os_setenv("UV_THREADPOOL_SIZE", std::to_string(config.cpu).c_str());
    Server *server = new Server(IP, PORT, config.root, config.cpu);
    return 0;
}