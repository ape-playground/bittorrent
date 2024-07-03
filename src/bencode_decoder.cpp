#include "bencode_decoder.h"
#include "lib/nlohmann/json.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cctype>
#include <cstdlib>

using json = nlohmann::json;

json decode_bencoded_string(const std::string &encoded_string, size_t &index) {
    size_t colon_pos = encoded_string.find(':', index);
    int64_t length = std::stoll(encoded_string.substr(index, colon_pos - index));
    index = colon_pos + 1 + length;
    return json(encoded_string.substr(colon_pos + 1, length)); // NOLINT
}

json decode_bencoded_integer(const std::string &encoded_integer, size_t &index) {
    size_t end_pos = encoded_integer.find('e', index);
    int64_t number = std::stoll(encoded_integer.substr(index + 1, end_pos - index - 1));
    index = end_pos + 1;
    return json(number); // NOLINT
}

// NOLINTNEXTLINE
json decode_bencoded_list(const std::string &encoded_value, size_t &index) {
    std::vector<json> decoded_list;

    while (index < encoded_value.size() && encoded_value[index] != 'e') {
        if (std::isdigit(encoded_value[index])) {
            decoded_list.push_back(decode_bencoded_string(encoded_value, index));
        } else if (encoded_value[index] == 'i') {
            decoded_list.push_back(decode_bencoded_integer(encoded_value, index));
        } else if (encoded_value[index] == 'l') {
            index++; // Skip 'l'
            decoded_list.push_back(decode_bencoded_list(encoded_value, index));
        } else {
            throw std::runtime_error("Invalid bencoded list element");
        }
    }
    index++; // Skip 'e'
    return decoded_list;
}

json decode_bencoded_value(const std::string &encoded_value) {
    size_t index = 0;
    if (encoded_value[0] == 'l') {
        index++;
        return decode_bencoded_list(encoded_value, index);
    } else if (encoded_value[0] == 'i') {
        return decode_bencoded_integer(encoded_value, index);
    } else if (std::isdigit(encoded_value[0])) {
        return decode_bencoded_string(encoded_value, index);
    } else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}
