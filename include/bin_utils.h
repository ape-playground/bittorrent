#ifndef BIN_UTILS_H
#define BIN_UTILS_H

#include <string>
#include <sstream>
#include <iomanip>

namespace BinUtils {

    // Base64 character set
    constexpr char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "abcdefghijklmnopqrstuvwxyz"
                                    "0123456789+/";

    /**
 * @brief Encodes binary data to a Base64 string.
 *
 * @param bytes_to_encode Pointer to the binary data.
 * @param in_len Length of the binary data.
 * @return Base64 encoded string.
 */
    inline std::string hBase64Encode(const unsigned char *bytes_to_encode, unsigned int in_len) {
        std::string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; i < 4; i++)
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

            for (j = 0; j < i + 1; j++)
                ret += base64_chars[char_array_4[j]];

            while (i++ < 3)
                ret += '=';
        }

        return ret;
    }

    /**
 * @brief Encodes binary data to a Base64 string.
 *
 * @param binaryData Binary data as a string.
 * @return Base64 encoded string.
 */
    inline std::string base64Encode(const std::string &binaryData) {
        return hBase64Encode(reinterpret_cast<const unsigned char *>(binaryData.data()), binaryData.size());
    }

    /**
 * @brief Converts binary data to a hexadecimal string.
 *
 * @param binaryData Binary data as a string.
 * @return Hexadecimal representation of the binary data.
 */
    inline std::string binaryToHex(const std::string &binaryData) {
        std::ostringstream hexStream;
        for (unsigned char c: binaryData) {
            hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        }
        return hexStream.str();
    }

} // namespace BinUtils

#endif // BIN_UTILS_H
