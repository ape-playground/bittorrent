#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <fstream>
#include <sstream>

namespace FileUtils {

    std::string read(const std::string &file_path) {
        std::ifstream file(file_path, std::ios::binary);
        std::stringstream buff;
        if (file) {
            buff << file.rdbuf();
            file.close();
            return buff.str();
        } else {
            throw std::runtime_error("Failed to open file: " + file_path);
        }
    }
}

#endif //FILE_UTILS_H
