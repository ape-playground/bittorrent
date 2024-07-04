#ifndef BENCODE_H
#define BENCODE_H

#include "lib/nlohmann/json.hpp"
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

class Bencode {
public:
    // Decoding methods
    explicit Bencode(const std::string &encoded);

    json decode();

    // Encoding methods
    explicit Bencode(const json &value);

    std::string encode();

private:
    // Decoding members
    const std::string *encoded_string;
    size_t index;

    // Encoding members
    const json *value;

    // Decoding helper methods
    json decodeString();

    json decodeInteger();

    json decodeList();

    json decodeDict();

    json decodeValue();

    // Encoding helper methods
    std::string encodeString(const std::string &str);

    std::string encodeInteger(int64_t value);

    std::string encodeList(const json &list);

    std::string encodeDict(const json &dict);

    std::string encodeValue(const json &value);
};

#endif // BENCODE_H
