#include "bencode/encoder.h"
#include <sstream>

BencodeEncoder::BencodeEncoder(const json &value) : value(value) {}

std::string BencodeEncoder::encode() {
    return encodeValue(value);
}

std::string BencodeEncoder::encodeString(const std::string &str) {
    return std::to_string(str.size()) + ":" + str;
}

std::string BencodeEncoder::encodeInteger(int64_t value) {
    return "i" + std::to_string(value) + "e";
}

std::string BencodeEncoder::encodeList(const json &list) {
    std::string encoded = "l";
    for (const auto &item: list) {
        encoded += encodeValue(item);
    }
    return encoded + "e";
}

std::string BencodeEncoder::encodeDict(const json &dict) {
    std::string encoded = "d";
    for (auto it = dict.begin(); it != dict.end(); ++it) {
        encoded += encodeString(it.key());
        encoded += encodeValue(it.value());
    }
    return encoded + "e";
}

std::string BencodeEncoder::encodeValue(const json &value) {
    if (value.is_string()) {
        return encodeString(value.get<std::string>());
    } else if (value.is_number_integer()) {
        return encodeInteger(value.get<int64_t>());
    } else if (value.is_array()) {
        return encodeList(value);
    } else if (value.is_object()) {
        return encodeDict(value);
    } else {
        throw std::runtime_error("Unsupported JSON value type for bencoding.");
    }
}
