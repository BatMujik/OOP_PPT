#include "Shape.h"
#include "Slide.h"
#include "SlideShow.h"
#include "Tokenizer.h"
#include "CommandParser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>

/**
 * @brief Loads a presentation file and creates a SlideShow
 * @param filename The path to the presentation file
 * @return A unique pointer to the SlideShow, or nullptr on failure
 */
std::unique_ptr<SlideShow> loadPresentation(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERR] Failed to open file: " << filename << std::endl;
        return nullptr;
    }

    auto slideshow = std::make_unique<SlideShow>(filename);
    Slide currentSlide;
    std::string line;
    bool hasContent = false;

    while (std::getline(file, line)) {
        std::string trimmedLine = Tokenizer::trim(line);
        
        // Check for slide separator
        if (trimmedLine == "---") {
            slideshow->addSlide(currentSlide);
            currentSlide = Slide();
            hasContent = false;
            continue;
        }
        
        // Skip empty lines
        if (trimmedLine.empty()) {
            continue;
        }
        
        // Parse shape: ShapeName, x, y
        std::vector<std::string> tokens = Tokenizer::split(trimmedLine, ',');
        if (tokens.size() == 3) {
            try {
                std::string shapeName = tokens[0];
                int x = std::stoi(tokens[1]);
                int y = std::stoi(tokens[2]);
                auto shape = std::make_shared<Shape>(shapeName, x, y);
                currentSlide.addShape(shape);
                hasContent = true;
            } catch (const std::exception& e) {
                std::cerr << "[WARN] Failed to parse line in " << filename << ": " << line << std::endl;
            }
        } else {
            std::cerr << "[WARN] Invalid line format in " << filename << ": " << line << std::endl;
        }
    }
    
    // Add the last slide if it has content
    if (hasContent || currentSlide.getShapeCount() > 0) {
        slideshow->addSlide(currentSlide);
    }

    file.close();
    
    if (slideshow->isEmpty()) {
        std::cerr << "[WARN] No slides found in: " << filename << std::endl;
    } else {
        std::cout << "[INFO] Loaded " << slideshow->getTotalSlides() << " slide(s) from: " << filename << std::endl;
    }

    return slideshow;
}

/**
 * @brief Displays the help message with available commands
 */
void displayHelp() {
    std::cout << "\n=== Available Commands ===" << std::endl;
    std::cout << "  next             - Move to the next slide in the current presentation" << std::endl;
    std::cout << "  prev             - Move to the previous slide in the current presentation" << std::endl;
    std::cout << "  show             - Display the current slide's shapes" << std::endl;
    std::cout << "  goto <n>         - Jump to slide number <n> (1-based) in the current presentation" << std::endl;
    std::cout << "  goto <file> <n>  - Jump to slide number <n> in the specified presentation" << std::endl;
    std::cout << "  nextfile         - Switch to the next loaded presentation" << std::endl;
    std::cout << "  prevfile         - Switch to the previous loaded presentation" << std::endl;
    std::cout << "  add              - Add a shape to the current slide (interactive)" << std::endl;
    std::cout << "  help             - Display this help message" << std::endl;
    std::cout << "  exit             - Exit the program" << std::endl;
    std::cout << "==========================\n" << std::endl;
}

/**
 * @brief Displays the prompt with current file and slide information
 */
void displayPrompt(const SlideShow* currentShow) {
    if (currentShow && !currentShow->isEmpty()) {
        std::cout << "[" << currentShow->getFilename() << ", Slide " 
                  << currentShow->getCurrentSlideNumber() << "/" 
                  << currentShow->getTotalSlides() << "] > ";
    } else {
        std::cout << "[No presentation] > ";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "[ERR] Usage: " << argv[0] << " <presentation1.txt> [presentation2.txt] ..." << std::endl;
        return 1;
    }

    // Load all presentations
    std::vector<std::unique_ptr<SlideShow>> slideshows;
    std::map<std::string, int> filenameToIndex;

    std::cout << "[INFO] Loading presentations..." << std::endl;
    for (int i = 1; i < argc; ++i) {
        std::string filename = argv[i];
        auto slideshow = loadPresentation(filename);
        if (slideshow && !slideshow->isEmpty()) {
            std::string normalizedName = CommandParser::normalizePath(filename);
            filenameToIndex[normalizedName] = static_cast<int>(slideshows.size());
            slideshows.push_back(std::move(slideshow));
        }
    }

    if (slideshows.empty()) {
        std::cerr << "[ERR] No valid presentations loaded. Exiting." << std::endl;
        return 1;
    }

    int currentShowIndex = 0;
    std::cout << "[INFO] " << slideshows.size() << " presentation(s) loaded successfully.\n" << std::endl;

    // Display initial slide
    slideshows[currentShowIndex]->show();

    // Main CLI loop
    std::string input;
    bool running = true;

    while (running) {
        displayPrompt(slideshows[currentShowIndex].get());
        
        if (!std::getline(std::cin, input)) {
            // Handle EOF (Ctrl+D on Unix, Ctrl+Z on Windows)
            break;
        }

        CommandParser::Command cmd = CommandParser::parse(input);

        switch (cmd.type) {
            case CommandParser::NEXT:
                if (slideshows[currentShowIndex]->next()) {
                    slideshows[currentShowIndex]->show();
                } else {
                    std::cout << "[INFO] Already at the last slide." << std::endl;
                }
                break;

            case CommandParser::PREV:
                if (slideshows[currentShowIndex]->prev()) {
                    slideshows[currentShowIndex]->show();
                } else {
                    std::cout << "[INFO] Already at the first slide." << std::endl;
                }
                break;

            case CommandParser::SHOW:
                slideshows[currentShowIndex]->show();
                break;

            case CommandParser::GOTO:
                try {
                    int slideNum = std::stoi(cmd.args[0]);
                    if (slideshows[currentShowIndex]->gotoSlide(slideNum)) {
                        slideshows[currentShowIndex]->show();
                    } else {
                        std::cout << "[ERR] Invalid slide number. Valid range: 1-" 
                                  << slideshows[currentShowIndex]->getTotalSlides() << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "[ERR] Invalid slide number format." << std::endl;
                }
                break;

            case CommandParser::GOTO_FILE:
                {
                    std::string targetFile = cmd.args[0];
                    auto it = filenameToIndex.find(targetFile);
                    
                    if (it != filenameToIndex.end()) {
                        try {
                            int slideNum = std::stoi(cmd.args[1]);
                            int targetIndex = it->second;
                            
                            if (slideshows[targetIndex]->gotoSlide(slideNum)) {
                                currentShowIndex = targetIndex;
                                slideshows[currentShowIndex]->show();
                            } else {
                                std::cout << "[ERR] Invalid slide number for " << targetFile 
                                          << ". Valid range: 1-" << slideshows[targetIndex]->getTotalSlides() << std::endl;
                            }
                        } catch (const std::exception& e) {
                            std::cout << "[ERR] Invalid slide number format." << std::endl;
                        }
                    } else {
                        std::cout << "[ERR] Presentation not found: " << targetFile << std::endl;
                        std::cout << "[INFO] Available presentations:" << std::endl;
                        for (const auto& pair : filenameToIndex) {
                            std::cout << "  - " << pair.first << std::endl;
                        }
                    }
                }
                break;

            case CommandParser::NEXTFILE:
                if (currentShowIndex < static_cast<int>(slideshows.size()) - 1) {
                    currentShowIndex++;
                    std::cout << "[INFO] Switched to: " << slideshows[currentShowIndex]->getFilename() << std::endl;
                    slideshows[currentShowIndex]->show();
                } else {
                    std::cout << "[INFO] Already at the last presentation." << std::endl;
                }
                break;

            case CommandParser::PREVFILE:
                if (currentShowIndex > 0) {
                    currentShowIndex--;
                    std::cout << "[INFO] Switched to: " << slideshows[currentShowIndex]->getFilename() << std::endl;
                    slideshows[currentShowIndex]->show();
                } else {
                    std::cout << "[INFO] Already at the first presentation." << std::endl;
                }
                break;

            case CommandParser::ADD:
                {
                    Slide* currentSlide = slideshows[currentShowIndex]->getCurrentSlide();
                    if (!currentSlide) {
                        std::cout << "[ERR] No current slide to add shape to." << std::endl;
                        break;
                    }
                    
                    // Display shape catalog
                    std::vector<std::string> shapes = CommandParser::getAvailableShapes();
                    std::cout << "\n=== Shape Catalog ===" << std::endl;
                    for (size_t i = 0; i < shapes.size(); ++i) {
                        std::cout << "  " << (i + 1) << ". " << shapes[i] << std::endl;
                    }
                    std::cout << "  " << (shapes.size() + 1) << ". Quit" << std::endl;
                    std::cout << "=====================" << std::endl;
                    std::cout << "Enter: <number> <x> <y> (e.g., '1 50 60')" << std::endl;
                    std::cout << "> ";
                    
                    // Get selection input
                    std::string selectionInput;
                    if (!std::getline(std::cin, selectionInput)) {
                        break;
                    }
                    
                    // Parse selection: <number> <x> <y>
                    std::vector<std::string> selectionTokens = CommandParser::tokenize(selectionInput);
                    if (selectionTokens.size() == 3) {
                        try {
                            int selection = std::stoi(selectionTokens[0]);
                            
                            // Check for quit
                            if (selection == static_cast<int>(shapes.size() + 1)) {
                                std::cout << "[INFO] Cancelled add operation." << std::endl;
                                break;
                            }
                            
                            // Validate selection
                            if (selection < 1 || selection > static_cast<int>(shapes.size())) {
                                std::cout << "[ERR] Invalid selection. Please choose 1-" << (shapes.size() + 1) << "." << std::endl;
                                break;
                            }
                            
                            std::string selectedShape = shapes[selection - 1];
                            int x = std::stoi(selectionTokens[1]);
                            int y = std::stoi(selectionTokens[2]);
                            
                            auto shape = std::make_shared<Shape>(selectedShape, x, y);
                            currentSlide->addShape(shape);
                            std::cout << "[INFO] Added " << selectedShape << " at (" << x << ", " << y << ") to current slide." << std::endl;
                            slideshows[currentShowIndex]->show();
                        } catch (const std::exception& e) {
                            std::cout << "[ERR] Invalid input format. Use: <number> <x> <y>" << std::endl;
                        }
                    } else if (selectionTokens.size() == 1) {
                        try {
                            int selection = std::stoi(selectionTokens[0]);
                            if (selection == static_cast<int>(shapes.size() + 1)) {
                                std::cout << "[INFO] Cancelled add operation." << std::endl;
                            } else {
                                std::cout << "[ERR] Missing coordinates. Use: <number> <x> <y>" << std::endl;
                            }
                        } catch (const std::exception& e) {
                            std::cout << "[ERR] Invalid input format. Use: <number> <x> <y>" << std::endl;
                        }
                    } else {
                        std::cout << "[ERR] Invalid input format. Use: <number> <x> <y>" << std::endl;
                    }
                }
                break;

            case CommandParser::HELP:
                displayHelp();
                break;

            case CommandParser::EXIT:
                std::cout << "[INFO] Exiting slideshow. Goodbye!" << std::endl;
                running = false;
                break;

            case CommandParser::INVALID:
                if (!Tokenizer::trim(input).empty()) {
                    std::cout << "[ERR] Unknown command. Type 'help' for available commands." << std::endl;
                }
                break;
        }
    }

    return 0;
}
