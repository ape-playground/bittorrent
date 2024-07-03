#ifndef READ_TORRENT_FILE_H
#define READ_TORRENT_FILE_H

#include <string>
#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json read_torrent_file(const std::string &file_path);

void torrent_info(const std::string &file_path);

#endif //READ_TORRENT_FILE_H