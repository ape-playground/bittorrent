#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json decode_bencoded_value(const std::string &encoded_value) {
    if (std::isdigit(encoded_value[0])) {
        // Example: "5:hello" -> "hello"
        size_t colon_index = encoded_value.find(':');
        if (colon_index != std::string::npos) {
            std::string number_string = encoded_value.substr(0, colon_index);
            int64_t number = std::strcoll(number_string.c_str(), nullptr);
            std::string str = encoded_value.substr(colon_index + 1, number);
            return json(str); // NOLINT
        } else {
            throw std::runtime_error("Invalid encoded value: " + encoded_value);
        }
    } else if (encoded_value[0] == 'i') {
        // Example: "i42e" -> 42
        if (encoded_value[encoded_value.size() - 1] != 'e') {
            throw std::runtime_error("Invalid encoded value: " + encoded_value);
        }
        std::string number_string = encoded_value.substr(1, encoded_value.size() - 2);
        int64_t number = std::strtoll(number_string.c_str(), nullptr, 10); // [<cint>]
        return json(number); // NOLINT
    } else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}

int main(int argc, char *argv[]) {
    // Flush after every std::cout / std::cerr
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
        json decoded_value = decode_bencoded_value(encoded_value);
        std::cout << decoded_value.dump() << std::endl;
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
