#include <iostream>
#include <string>
#include "lib/nlohmann/json.hpp"
#include "bencode.h"
#include "read_torrent_file.h"

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "decode") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
            return 1;
        }
        std::string encoded_value = argv[2];
        Bencode decoder(encoded_value);
        json decoded_value = decoder.decode();
        std::cout << decoded_value.dump() << std::endl;
    } else if (command == "info") {
        std::string filename = argv[2];
        TorrentInfo info = torrentInfo(filename);
        std::cout << "Tracker URL: " << info.tracker_url << std::endl;
        std::cout << "Length: " << info.file_length << std::endl;
        std::cout << "Info Hash: " << info.info_hash << std::endl;
        std::cout << "Piece Length: " << info.piece_length << std::endl;
        std::cout << "Piece Hashes:" << std::endl;
        for (const std::string &hash: info.piece_hashes) {
            std::cout << hash << std::endl;
        }
    } else if (command == "print") {
        std::string filename = argv[2];
        printTorrentContent(filename);
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
