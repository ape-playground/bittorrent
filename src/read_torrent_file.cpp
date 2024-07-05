#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "utils/file.h"
#include "bencode.h"
#include "utils/binary.h"
#include "lib/nlohmann/json.hpp"
#include "utils/sha1.h"
#include "read_torrent_file.h"

using json = nlohmann::json;


json readTorrentFile(const std::string &torrentFilePath) {
    std::string encoded_value = FileUtils::read(torrentFilePath);
    Bencode decoder(encoded_value);
    return decoder.decode();
}

TorrentInfo torrentInfo(const std::string &torrentFilePath) {
    TorrentInfo info;

    try {
        json decoded_value = readTorrentFile(torrentFilePath);
        info.tracker_url = decoded_value["announce"];
        info.file_length = decoded_value["info"]["length"];
        info.piece_length = decoded_value["info"]["piece length"];
        std::string pieces = decoded_value["info"]["pieces"];

        Bencode encoder(decoded_value["info"]);
        std::string encoded_info = encoder.encode();
        info.info_hash = sha1(encoded_info);

        for (std::size_t i = 0; i < pieces.length(); i += 20) {
            std::string piece = pieces.substr(i, 20);
            std::stringstream ss;
            for (unsigned char byte: piece) {
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
            }
            info.piece_hashes.push_back(ss.str());
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;  // Rethrow the exception after logging it
    }

    return info;
}

void printTorrentContent(const std::string &torrentFilePath) {
    //{
    // "announce":"http://bittorrent-test-tracker.codecrafters.io/announce",
    // "created by":"mktorrent 1.1",
    // "info":{
    //  "length":92063,
    //  "name":"sample.txt",
    //  "piece length":32768,
    //  "pieces": <binary data> #6Hb2eiqIhujzaxNnJsMPopcDAi1uInXmBKB2ZlZzboH/ELVSBK2NNfANk3oCE98ZgryNCXInrZ6QmswX5Q==
    // }
    //}

    try {
        json decoded_value = readTorrentFile(torrentFilePath);
        // Process the "pieces" field
        if (decoded_value.contains("info") && decoded_value["info"].contains("pieces")) {
            std::string pieces_binary = decoded_value["info"]["pieces"].get<std::string>();
            std::string pieces_64 = BinUtils::base64Encode(pieces_binary);
            decoded_value["info"]["pieces"] = pieces_64; // Replace binary data with hex string
        }

        std::cout << decoded_value.dump(4) << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}