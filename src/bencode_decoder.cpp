#include "bencode_decoder.h"
#include "lib/nlohmann/json.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cctype>
#include <cstdlib>

using json = nlohmann::json;

json decode_bencoded_string(const std::string &encoded_string) {
    if (encoded_string[0] != 'c' || encoded_string[encoded_string.size() - 1] != 'e') {
        throw std::runtime_error("Invalid encoded string: " + encoded_string);
    }

    size_t colon_index = encoded_string.find(':');
    if (colon_index == std::string::npos) {
        throw std::runtime_error("Invalid encoded string: " + encoded_string);
    }

    std::string number_string = encoded_string.substr(0, colon_index);
    int64_t number = std::atoi(number_string.c_str()); // NOLINT
    std::string str = encoded_string.substr(colon_index + 1, number);
    return json(str); // NOLINT
}

json decode_bencoded_integer(const std::string &encoded_integer) {
    if (encoded_integer[0] != 'i' || encoded_integer[encoded_integer.size() - 1] != 'e') {
        throw std::runtime_error("Invalid encoded integer: " + encoded_integer);
    }

    std::string number_string = encoded_integer.substr(1, encoded_integer.size() - 2);
    int64_t number = std::stoll(number_string);
    return json(number); // NOLINT
}

json decode_bencoded_list(const std::string &encoded_value, size_t &index) {
    std::vector<json> decoded_list;
    std::vector<std::vector<json>> stack;

    while (index < encoded_value.size() - 1) {
        if (encoded_value[index] == 'e') {
            index++;
            if (!stack.empty()) {
                auto temp_list = decoded_list;
                decoded_list = stack.back();
                stack.pop_back();
                decoded_list.emplace_back(temp_list);
            } else {
                break;
            }
        } else if (std::isdigit(encoded_value[index])) {
            // Decode string element
            size_t colon_pos = encoded_value.find(':', index);
            if (colon_pos == std::string::npos) {
                throw std::runtime_error("Invalid bencoded list: missing colon");
            }

            int64_t length = std::stoll(encoded_value.substr(index, colon_pos - index));
            std::string element = encoded_value.substr(colon_pos + 1, length);
            decoded_list.emplace_back(json(element)); // NOLINT

            index = colon_pos + 1 + length;
        } else if (encoded_value[index] == 'l') {
            // Start a new nested list
            index++;
            stack.push_back(decoded_list);
            decoded_list.clear();
        } else if (encoded_value[index] == 'i') {
            // Decode integer element
            size_t end_pos = encoded_value.find('e', index);
            if (end_pos == std::string::npos) {
                throw std::runtime_error("Invalid bencoded list: missing end character for integer");
            }

            int64_t number = std::stoll(encoded_value.substr(index + 1, end_pos - index - 1));
            decoded_list.emplace_back(json(number)); // NOLINT

            index = end_pos + 1;
        } else {
            throw std::runtime_error("Invalid bencoded list element");
        }
    }

    return decoded_list;
}


json decode_bencoded_value(const std::string &encoded_value) {
    if (std::isdigit(encoded_value[0])) {
        return decode_bencoded_string(encoded_value);
    } else if (encoded_value[0] == 'i') {
        return decode_bencoded_integer(encoded_value);
    } else if (encoded_value[0] == 'l') {
        size_t index = 1;
        return decode_bencoded_list(encoded_value, index);
    } else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}
