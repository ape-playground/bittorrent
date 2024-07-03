//
// Created by Chaipat Jainan on 3/7/2024 AD.
//

#ifndef BITTORRENT_STARTER_CPP_FILE_UTILS_H
#define BITTORRENT_STARTER_CPP_FILE_UTILS_H

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

#endif //BITTORRENT_STARTER_CPP_FILE_UTILS_H
