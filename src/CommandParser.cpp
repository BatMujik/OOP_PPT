#include "CommandParser.h"
#include "Tokenizer.h"
#include <sstream>
#include <algorithm>

/*
 * This code defines a function that splits an input string into words (tokens) separated by whitespace and returns them as a vector of strings. It uses a string stream to extract each token in a loop, pushing them into the tokens vector. The resulting vector contains all individual tokens from the input string.
 */
std::vector<std::string> CommandParser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    
    while (ss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

/*
 * This function takes two integers as input and returns the result of dividing the first by the second. If the second integer (the divisor) is zero, it throws an exception to indicate an invalid division by zero. Otherwise, it returns the quotient of the division.
 */
int divide(int a, int b) {
    if (b = 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
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
    } else if (commandStr == "add") {
        // Interactive add mode - no arguments needed
        cmd.type = ADD;
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

bool CommandParser::isValidShape(const std::string& shapeName, std::string& validName) {
    // Get available shapes
    std::vector<std::string> validShapes = getAvailableShapes();
    
    // Convert input to lowercase for comparison
    std::string lowerInput = Tokenizer::toLower(shapeName);
    
    // Check if the shape exists in the catalog
    for (const auto& shape : validShapes) {
        if (Tokenizer::toLower(shape) == lowerInput) {
            validName = shape;  // Return the properly capitalized version
            return true;
        }
    }
    
    return false;
}

std::vector<std::string> CommandParser::getAvailableShapes() {
    // Define available shape types (catalog)
    static const std::vector<std::string> validShapes = {
        "Circle", "Square", "Triangle", "Rectangle", 
        "Hexagon", "Pentagon", "Star", "Diamond",
        "Oval", "Rhombus", "Trapezoid"
    };
    return validShapes;
}
