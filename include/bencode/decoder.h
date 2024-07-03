#ifndef BENCODE_DECODER_H
#define BENCODE_DECODER_H

#include <string>
#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json decode_bencoded_string(const std::string &encoded_string, size_t &index);

json decode_bencoded_integer(const std::string &encoded_integer, size_t &index);

json decode_bencoded_list(const std::string &encoded_value, size_t &index);

json decode_bencoded_dict(const std::string &encoded_value, size_t &index);

json decoder(const std::string &encoded_value, size_t &index);

json decode_bencoded_value(const std::string &encoded_value);


#endif // BENCODE_DECODER_H
