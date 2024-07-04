#ifndef BENCODE_DECODER_H
#define BENCODE_DECODER_H


#include "lib/nlohmann/json.hpp"
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

class BencodeDecoder {
public:
    explicit BencodeDecoder(const std::string &encoded);

    json decode();

private:
    const std::string &encoded_string;
    size_t index;

    json decodeString();

    json decodeInteger();

    json decodeList();

    json decodeDict();

    json decodeValue();
};

#endif // BENCODE_DECODER_H
