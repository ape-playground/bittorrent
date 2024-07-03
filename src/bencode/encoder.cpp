#include "bencode/encoder.h"
#include "lib/nlohmann/json.hpp"
#include <iostream>
#include <sstream>


using json = nlohmann::json;

namespace encoder {
    std::string encode_bencoded_string(const json &decoded_string) {
        std::ostringstream os;
        const std::string &str = decoded_string.get<std::string>();
        os << str.size() << ":" << str;
        return os.str();
    }

    std::string encode_bencoded_integer(const json &decoded_integer) {
        std::ostringstream os;
        os << "i" << decoded_integer.get<int>() << "e";
        return os.str();
    }

    std::string encode_bencoded_list(const json &decoded_list) {
        std::ostringstream os;
        os << "l";
        for (const auto &item: decoded_list) {
            os << encode(item);
        }
        os << "e";
        return os.str();
    }

    std::string encode_bencoded_dict(const json &decoded_dict) {
        std::ostringstream os;
        os << "d";
        for (const auto &item: decoded_dict.items()) {
            os << item.key().size() << ":" << item.key() << encode(item.value());
        }
        os << "e";
        return os.str();
    }

    std::string encode(const json &decoded_value) {
        std::ostringstream os;
        if (decoded_value.is_string()) {
            os << encode_bencoded_string(decoded_value);
        } else if (decoded_value.is_number()) {
            os << encode_bencoded_integer(decoded_value);
        } else if (decoded_value.is_array()) {
            os << encode_bencoded_list(decoded_value);
        } else if (decoded_value.is_object()) {
            os << encode_bencoded_dict(decoded_value);
        } else {
            throw std::invalid_argument("Invalid type");
        }
        return os.str();

    }
}
