#ifndef BENCODE_ENCODER_H
#define BENCODE_ENCODER_H

#include <string>
#include "lib/nlohmann/json.hpp"

namespace encoder {
    std::string encode_bencoded_string(const nlohmann::json &decoded_string);

    std::string encode_bencoded_integer(const nlohmann::json &decoded_integer);

    std::string encode_bencoded_list(const nlohmann::json &decoded_list);

    std::string encode_bencoded_dict(const nlohmann::json &decoded_dict);

    std::string encode(const nlohmann::json &decoded_value);
}


#endif //BENCODE_ENCODER_H
