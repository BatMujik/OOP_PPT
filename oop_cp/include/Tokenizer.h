#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

/**
 * @brief Utility class for tokenizing strings with delimiter support and trimming
 */
class Tokenizer {
public:
    /**
     * @brief Splits a string by a delimiter
     * @param str The string to split
     * @param delimiter The delimiter character (e.g., ',')
     * @return A vector of tokens
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);

    /**
     * @brief Trims leading and trailing whitespace from a string
     * @param str The string to trim
     * @return The trimmed string
     */
    static std::string trim(const std::string& str);

    /**
     * @brief Converts a string to lowercase
     * @param str The string to convert
     * @return The lowercase string
     */
    static std::string toLower(const std::string& str);
};

#endif // TOKENIZER_H
