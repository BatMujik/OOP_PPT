#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <vector>

/**
 * @brief Parses and validates user commands for the slideshow application
 */
class CommandParser {
public:
    /**
     * @brief Enumeration of all supported command types
     */
    enum CommandType {
        // Navigation commands
        NEXT,
        PREV,
        SHOW,
        GOTO,
        GOTO_FILE,
        NEXTFILE,
        PREVFILE,
        
        // Shape manipulation
        ADD,
        DELETE,
        EDIT,
        LIST,
        CLEAR,
        
        // Slide manipulation
        NEWSLIDE,
        DELSLIDE,
        
        // View modes
        VISUAL,
        TEXT,
        
        // Export commands
        EXPORT,
        EXPORT_ALL,
        
        // Theme/appearance
        THEME,
        COLORS,
        SHAPES,
        
        // File operations
        NEW,
        SAVE,
        SAVEAS,
        
        // System commands
        HELP,
        EXIT,
        
        // Invalid/unknown
        INVALID
    };

    /**
     * @brief Structure to hold parsed command data
     */
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
     * @brief Tokenizes the input string into words
     * @param input The input string
     * @return A vector of tokens
     */
    static std::vector<std::string> tokenize(const std::string& input);
};

#endif // COMMANDPARSER_H
