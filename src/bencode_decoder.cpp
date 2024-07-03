#include "bencode_decoder.h"
#include "lib/nlohmann/json.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdlib>

using json = nlohmann::json;

/**
 * Decode a bencoded string from the given position in the encoded string.
 * Updates the index to point past the decoded string.
 *
 * PATTERNS: <length>:<string>
 * SAMPLE: 3:foo
 * RESULT: "foo"
 */
json decode_bencoded_string(const std::string &encoded_string, size_t &index) {
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

/**
 * Decode a bencoded integer from the given position in the encoded string.
 * Updates the index to point past the decoded integer.
 *
 * PATTERNS: i<value>e
 * SAMPLE: i42e
 * RESULT: 42
 */
json decode_bencoded_integer(const std::string &encoded_integer, size_t &index) {
    size_t start = index + 1; // skip 'i'
    size_t end = encoded_integer.find('e', start);
    if (end == std::string::npos) {
        throw std::runtime_error("Invalid integer encoding: 'e' not found.");
    }

    std::string num_str = encoded_integer.substr(start, end - start);
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

/**
 * Decode a bencoded list from the given position in the encoded string.
 * Updates the index to point past the decoded list.
 *
 * PATTERNS: l<value>e
 * SAMPLE: l3:foo3:bare
 * RESULT: ["foo", "bar"]
 */
json decode_bencoded_list(const std::string &encoded_value, size_t &index) {
    index++; // skip 'l'
    std::vector<json> list;

    while (index < encoded_value.length() && encoded_value[index] != 'e') {
        list.push_back(decoder(encoded_value, index));
    }
    index++; // skip 'e'
    return json(list);
}

/**
 * Decode a bencoded dictionary from the given position in the encoded string.
 * Updates the index to point past the decoded dictionary.
 *
 * PATTERNS: d<key><value>e
 * SAMPLE: d3:foo3:bare
 * RESULT: {"foo": "bar"}
 */
json decode_bencoded_dict(const std::string &encoded_value, size_t &index) {
    index++; // skip 'd'
    json dict = json::object();

    while (index < encoded_value.length() && encoded_value[index] != 'e') {
        json key = decode_bencoded_string(encoded_value, index);
        json value = decoder(encoded_value, index);
        dict[key.get<std::string>()] = value;
    }
    index++; // skip 'e'
    return dict;
}

/**
 * Decode a bencoded value of any type from the given position in the encoded string.
 * Updates the index to point past the decoded value.
 */
json decoder(const std::string &encoded_value, size_t &index) {
    char type = encoded_value[index];
    switch (type) {
        case 'i':
            return decode_bencoded_integer(encoded_value, index);
        case 'l':
            return decode_bencoded_list(encoded_value, index);
        case 'd':
            return decode_bencoded_dict(encoded_value, index);
        case '0' ... '9':
            return decode_bencoded_string(encoded_value, index);
        default:
            throw std::runtime_error("Invalid bencoded value type.");
    }
}


json decode_bencoded_value(const std::string &encoded_value) {
    size_t index = 0;
    return decoder(encoded_value, index);
}
