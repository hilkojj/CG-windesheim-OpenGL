#include "files.h"

#include <fstream>
#include <sstream>

std::string files::readString(const char* path)
{
    std::ifstream stream(path);

    if (!stream.is_open())
        throw std::runtime_error("Could not open: " + std::string(path));

    std::stringstream sstr;
    sstr << stream.rdbuf();
    stream.close();
    return sstr.str();
}
