#include "bencode.h"

// Constructor for decoding
Bencode::Bencode(const std::string &encoded) : encoded_string(&encoded), index(0), value(nullptr) {}

// Constructor for encoding
Bencode::Bencode(const json &value) : encoded_string(nullptr), index(0), value(&value) {}

json Bencode::decode() {
    if (!encoded_string) {
        throw std::runtime_error("Decoder is not initialized with an encoded string.");
    }
    return decodeValue();
}

std::string Bencode::encode() {
    if (!value) {
        throw std::runtime_error("Encoder is not initialized with a JSON value.");
    }
    return encodeValue(*value);
}

// Decoding helper methods
json Bencode::decodeString() {
    size_t colon = encoded_string->find(':', index);
    if (colon == std::string::npos) {
        throw std::runtime_error("Invalid string encoding: colon not found.");
    }

    int length = std::stoi(encoded_string->substr(index, colon - index));
    if (length < 0) {
        throw std::runtime_error("Invalid string encoding: negative length.");
    }

    std::string result = encoded_string->substr(colon + 1, length);
    index = colon + 1 + length;
    return json(result);
}

json Bencode::decodeInteger() {
    size_t start = index + 1; // skip 'i'
    size_t end = encoded_string->find('e', start);
    if (end == std::string::npos) {
        throw std::runtime_error("Invalid integer encoding: 'e' not found.");
    }

    std::string num_str = encoded_string->substr(start, end - start);
    try {
        int64_t num = std::stoll(num_str);
        index = end + 1; // skip 'e'
        return json(num);
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Invalid integer encoding: " + std::string(e.what()));
    } catch (const std::out_of_range &e) {
        throw std::runtime_error("Integer out of range: " + std::string(e.what()));
    }
}

json Bencode::decodeList() {
    index++; // skip 'l'
    std::vector<json> list;

    while (index < encoded_string->length() && (*encoded_string)[index] != 'e') {
        list.push_back(decodeValue());
    }
    index++; // skip 'e'
    return json(list);
}

json Bencode::decodeDict() {
    index++; // skip 'd'
    json dict = json::object();

    while (index < encoded_string->length() && (*encoded_string)[index] != 'e') {
        json key = decodeString();
        json value = decodeValue();
        dict[key.get<std::string>()] = value;
    }
    index++; // skip 'e'
    return dict;
}

json Bencode::decodeValue() {
    char type = (*encoded_string)[index];
    switch (type) {
        case 'i':
            return decodeInteger();
        case 'l':
            return decodeList();
        case 'd':
            return decodeDict();
        case '0' ... '9':
            return decodeString();
        default:
            throw std::runtime_error("Invalid bencoded value type.");
    }
}

// Encoding helper methods
std::string Bencode::encodeString(const std::string &str) {
    return std::to_string(str.size()) + ":" + str;
}

std::string Bencode::encodeInteger(int64_t value) {
    return "i" + std::to_string(value) + "e";
}

std::string Bencode::encodeList(const json &list) {
    std::string encoded = "l";
    for (const auto &item: list) {
        encoded += encodeValue(item);
    }
    return encoded + "e";
}

std::string Bencode::encodeDict(const json &dict) {
    std::string encoded = "d";
    for (auto it = dict.begin(); it != dict.end(); ++it) {
        encoded += encodeString(it.key());
        encoded += encodeValue(it.value());
    }
    return encoded + "e";
}

std::string Bencode::encodeValue(const json &value) {
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
