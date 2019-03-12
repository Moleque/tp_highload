#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <sstream>
#include <fstream>

struct Settings {
    unsigned short cpu = 4;
    unsigned short thread = 256;
    std::string root = "/var/www/html";
};

int parseConfig(const std::string, struct Settings*);

#endif  // CONFIG_HPP
