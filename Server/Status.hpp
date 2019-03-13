#ifndef STATUS_HPP
#define STATUS_HPP

#include <map>

#define OK                  200
#define FORBIDDEN           403
#define NOT_FOUND           404
#define METHOD_NOT_ALLOWED  405

const std::map<const unsigned short, std::string> statusPhrase = {
    { OK, "OK" },
    { FORBIDDEN, "FORBIDDEN" },
    { NOT_FOUND, "NOT FOUND" },
    { METHOD_NOT_ALLOWED, "METHOD NOT ALLOWED" },
};

#endif  // STATUS_HPP
