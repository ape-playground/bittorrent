#include <iostream>
#include <iomanip>
#include <sstream>
#include "file_utils.h"
#include "bencode/decoder.h"
#include "bencode/encoder.h"
#include "lib/nlohmann/json.hpp"
#include "sha_utils.h"

using json = nlohmann::json;

json read_torrent_file(const std::string &file_path) {
    std::string encoded_value = FileUtils::read(file_path);
    BencodeDecoder decoder(encoded_value);
    return decoder.decode();
}

void torrent_info(const std::string &file_path) {
    try {
        json decoded_value = read_torrent_file(file_path);
        std::string tracker_url = decoded_value["announce"];
        int file_length = decoded_value["info"]["length"];
        int piece_length = decoded_value["info"]["piece length"];
        std::string pieces = decoded_value["info"]["pieces"];


        BencodeEncoder encoder(decoded_value["info"]);
        std::string encoded_info = encoder.encode();

        std::cout << "Tracker URL: " << tracker_url << std::endl;
        std::cout << "File Length: " << file_length << " bytes" << std::endl;
        std::cout << "Info Hash: " << sha1(encoded_info) << std::endl;
        std::cout << "Piece Length: " << piece_length << " bytes" << std::endl;
        std::cout << "Piece Hashes: " << std::endl;

        for (std::size_t i = 0; i < pieces.length(); i += 20) {
            std::string piece = pieces.substr(i, 20);
            std::stringstream ss;
            for (unsigned char byte: piece) {
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
            }
            std::cout << ss.str() << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
