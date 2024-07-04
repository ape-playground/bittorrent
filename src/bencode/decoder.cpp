#include "bencode/decoder.h"

BencodeDecoder::BencodeDecoder(const std::string &encoded) : encoded_string(encoded), index(0) {}

json BencodeDecoder::decode() {
    return decodeValue();
}

json BencodeDecoder::decodeString() {
    size_t colon = encoded_string.find(':', index);
    if (colon == std::string::npos) {
        throw std::runtime_error("Invalid string encoding: colon not found.");
    }

    int length = std::stoi(encoded_string.substr(index, colon - index));
    if (length < 0) {
        throw std::runtime_error("Invalid string encoding: negative length.");
    }

    std::string result = encoded_string.substr(colon + 1, length);
    index = colon + 1 + length;
    return json(result);
}

json BencodeDecoder::decodeInteger() {
    size_t start = index + 1; // skip 'i'
    size_t end = encoded_string.find('e', start);
    if (end == std::string::npos) {
        throw std::runtime_error("Invalid integer encoding: 'e' not found.");
    }

    std::string num_str = encoded_string.substr(start, end - start);
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

json BencodeDecoder::decodeList() {
    index++; // skip 'l'
    std::vector<json> list;

    while (index < encoded_string.length() && encoded_string[index] != 'e') {
        list.push_back(decodeValue());
    }
    index++; // skip 'e'
    return json(list);
}

json BencodeDecoder::decodeDict() {
    index++; // skip 'd'
    json dict = json::object();

    while (index < encoded_string.length() && encoded_string[index] != 'e') {
        json key = decodeString();
        json value = decodeValue();
        dict[key.get<std::string>()] = value;
    }
    index++; // skip 'e'
    return dict;
}

json BencodeDecoder::decodeValue() {
    char type = encoded_string[index];
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
