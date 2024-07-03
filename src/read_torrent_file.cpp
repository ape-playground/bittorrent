#include <iostream>
#include "file_utils.h"
#include "bencode_decoder.h"
#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * Read the torrent file and decode the bencoded value.
 *
 * @param file_path The path to the torrent file.
 */
json read_torrent_file(const std::string &file_path) {
    std::string encoded_torrent = FileUtils::read(file_path);
    return decode_bencoded_value(encoded_torrent);
}

/**
 * Print the tracker URL and the length of the file in the torrent file.
 *
 * @param file_path The path to the torrent file.
 */
void torrent_info(const std::string &file_path) {
    json decoded_value = read_torrent_file(file_path);

    auto URL = decoded_value["announce"].get<std::string>();
    auto LENGTH = decoded_value["info"]["length"].get<int>();

    std::cout << "Tracker URL: " << URL << std::endl;
    std::cout << "Length: " << LENGTH << std::endl;
}