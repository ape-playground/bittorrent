#ifndef BENCODE_ENCODER_H
#define BENCODE_ENCODER_H


#include "lib/nlohmann/json.hpp"
#include <string>
#include <vector>

using json = nlohmann::json;

class BencodeEncoder {
public:
    explicit BencodeEncoder(const json &value);

    std::string encode();

private:
    const json &value;

    std::string encodeString(const std::string &str);

    std::string encodeInteger(int64_t value);

    std::string encodeList(const json &list);

    std::string encodeDict(const json &dict);

    std::string encodeValue(const json &value);
};

#endif //BENCODE_ENCODER_H
