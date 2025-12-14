#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <vector>

/**
 * @brief Parses and validates user commands
 */
class CommandParser {
public:
    enum CommandType {
        NEXT,
        PREV,
        SHOW,
        GOTO,
        GOTO_FILE,
        NEXTFILE,
        PREVFILE,
        ADD,
        HELP,
        EXIT,
        INVALID
    };

    struct Command {
        CommandType type;
        std::vector<std::string> args;
    };

    /**
     * @brief Parses a command string into a Command structure
     * @param input The raw input string from the user
     * @return A Command structure with type and arguments
     */
    static Command parse(const std::string& input);

    /**
     * @brief Normalizes a file path (lowercase, forward slashes, strip ./ prefix)
     * @param path The path to normalize
     * @return The normalized path
     */
    static std::string normalizePath(const std::string& path);

    /**
     * @brief Checks if a shape name is valid and returns the properly capitalized version
     * @param shapeName The shape name to validate
     * @param validName Output parameter for the valid capitalized name
     * @return True if the shape is valid, false otherwise
     */
    static bool isValidShape(const std::string& shapeName, std::string& validName);

    /**
     * @brief Gets the list of available shape types
     * @return Vector of valid shape names
     */
    static std::vector<std::string> getAvailableShapes();

    /**
     * @brief Tokenizes the input string into words
     * @param input The input string
     * @return A vector of tokens
     */
    static std::vector<std::string> tokenize(const std::string& input);

private:
};

#endif // COMMANDPARSER_H
