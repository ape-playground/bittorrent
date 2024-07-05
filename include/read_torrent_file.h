#ifndef READ_TORRENT_FILE_H
#define READ_TORRENT_FILE_H

#include <string>
#include <vector>
#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

struct TorrentInfo {
    std::string tracker_url;
    std::string info_hash;

    int file_length;
    int piece_length;

    std::vector<std::string> piece_hashes;
};


json readTorrentFile(const std::string &torrentFilePath);

void printTorrentContent(const std::string &torrentFilePath);

TorrentInfo torrentInfo(const std::string &torrentFilePath);

#endif //READ_TORRENT_FILE_H