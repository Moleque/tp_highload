#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <sstream>
#include <fstream>

struct Settings {
    unsigned cpu;
    unsigned thread;
    std::string root;
};

int parseConfig(const std::string, struct Settings*);

#endif  // CONFIG_HPP
