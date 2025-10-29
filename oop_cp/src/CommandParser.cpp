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
    
    if (commandStr == "next") {
        cmd.type = NEXT;
    } else if (commandStr == "prev") {
        cmd.type = PREV;
    } else if (commandStr == "show") {
        cmd.type = SHOW;
    } else if (commandStr == "goto") {
        if (tokens.size() == 2) {
            // goto <n>
            cmd.type = GOTO;
            cmd.args.push_back(tokens[1]);
        } else if (tokens.size() == 3) {
            // goto <filename> <n>
            cmd.type = GOTO_FILE;
            cmd.args.push_back(normalizePath(tokens[1]));
            cmd.args.push_back(tokens[2]);
        } else {
            cmd.type = INVALID;
        }
    } else if (commandStr == "nextfile") {
        cmd.type = NEXTFILE;
    } else if (commandStr == "prevfile") {
        cmd.type = PREVFILE;
    } else if (commandStr == "help") {
        cmd.type = HELP;
    } else if (commandStr == "exit") {
        cmd.type = EXIT;
    } else {
        cmd.type = INVALID;
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
