#include "CommandParser.h"
#include "Tokenizer.h"
#include <sstream>
#include <algorithm>

std::vector<std::string> CommandParser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    
    while (ss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

CommandParser::Command CommandParser::parse(const std::string& input) {
    Command cmd;
    cmd.type = INVALID;
    
    std::string trimmedInput = Tokenizer::trim(input);
    if (trimmedInput.empty()) {
        return cmd;
    }
    
    std::vector<std::string> tokens = tokenize(trimmedInput);
    if (tokens.empty()) {
        return cmd;
    }
    
    std::string commandStr = Tokenizer::toLower(tokens[0]);
    
    // Copy all arguments after the command
    for (size_t i = 1; i < tokens.size(); ++i) {
        cmd.args.push_back(tokens[i]);
    }
    
    // Navigation commands
    if (commandStr == "next" || commandStr == "n") {
        cmd.type = NEXT;
    } else if (commandStr == "prev" || commandStr == "p" || commandStr == "previous") {
        cmd.type = PREV;
    } else if (commandStr == "show" || commandStr == "s") {
        cmd.type = SHOW;
    } else if (commandStr == "goto" || commandStr == "g") {
        if (tokens.size() == 2) {
            cmd.type = GOTO;
        } else if (tokens.size() == 3) {
            cmd.type = GOTO_FILE;
            cmd.args[0] = normalizePath(cmd.args[0]);
        }
    } else if (commandStr == "nextfile" || commandStr == "nf") {
        cmd.type = NEXTFILE;
    } else if (commandStr == "prevfile" || commandStr == "pf") {
        cmd.type = PREVFILE;
    }
    // Shape manipulation
    else if (commandStr == "add" || commandStr == "a") {
        cmd.type = ADD;
    } else if (commandStr == "delete" || commandStr == "del" || commandStr == "remove" || commandStr == "rm") {
        cmd.type = DELETE;
    } else if (commandStr == "edit" || commandStr == "e" || commandStr == "modify") {
        cmd.type = EDIT;
    } else if (commandStr == "list" || commandStr == "ls" || commandStr == "l") {
        cmd.type = LIST;
    } else if (commandStr == "clear") {
        cmd.type = CLEAR;
    }
    // Slide manipulation
    else if (commandStr == "newslide" || commandStr == "ns" || commandStr == "addslide") {
        cmd.type = NEWSLIDE;
    } else if (commandStr == "delslide" || commandStr == "ds" || commandStr == "removeslide") {
        cmd.type = DELSLIDE;
    }
    // View modes
    else if (commandStr == "visual" || commandStr == "v" || commandStr == "view") {
        cmd.type = VISUAL;
    } else if (commandStr == "text" || commandStr == "t") {
        cmd.type = TEXT;
    }
    // Export commands
    else if (commandStr == "export" || commandStr == "exp") {
        cmd.type = EXPORT;
    } else if (commandStr == "exportall" || commandStr == "expall") {
        cmd.type = EXPORT_ALL;
    } else if (commandStr == "exportjson" || commandStr == "expjson" || commandStr == "json") {
        cmd.type = EXPORT_JSON;
    }
    // Theme/appearance
    else if (commandStr == "theme") {
        cmd.type = THEME;
    } else if (commandStr == "colors") {
        cmd.type = COLORS;
    } else if (commandStr == "shapes") {
        cmd.type = SHAPES;
    }
    // File operations
    else if (commandStr == "new") {
        cmd.type = NEW;
    } else if (commandStr == "save") {
        cmd.type = SAVE;
    } else if (commandStr == "saveas") {
        cmd.type = SAVEAS;
    }
    // Undo/Redo commands
    else if (commandStr == "undo" || commandStr == "u" || commandStr == "z") {
        cmd.type = UNDO;
    } else if (commandStr == "redo" || commandStr == "r" || commandStr == "y") {
        cmd.type = REDO;
    } else if (commandStr == "history" || commandStr == "hist") {
        cmd.type = HISTORY;
    }
    // System commands
    else if (commandStr == "help" || commandStr == "h" || commandStr == "?") {
        cmd.type = HELP;
    } else if (commandStr == "exit" || commandStr == "quit" || commandStr == "q") {
        cmd.type = EXIT;
    }
    
    return cmd;
}

std::string CommandParser::normalizePath(const std::string& path) {
    std::string normalized = path;
    
    // Convert to lowercase
    normalized = Tokenizer::toLower(normalized);
    
    // Replace backslashes with forward slashes
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    
    // Strip leading "./"
    if (normalized.size() >= 2 && normalized.substr(0, 2) == "./") {
        normalized = normalized.substr(2);
    }
    
    return normalized;
}
