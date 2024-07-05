#ifndef SHA_UTILS_H
#define SHA_UTILS_H

#include <string>
#include <sstream>
#include <iomanip>
#include "lib/sha1.hpp"

std::string sha1(const std::string &data) {
    SHA1 checksum;
    checksum.update(data);
    return checksum.final();
}

#endif //SHA_UTILS_H
