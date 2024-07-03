//
// Created by Chaipat Jainan on 3/7/2024 AD.
//

#ifndef BITTORRENT_STARTER_CPP_READ_TORRENT_FILE_H
#define BITTORRENT_STARTER_CPP_READ_TORRENT_FILE_H

#include <string>
#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json read_torrent_file(const std::string &file_path);

void torrent_info(const std::string &file_path);

#endif //BITTORRENT_STARTER_CPP_READ_TORRENT_FILE_H