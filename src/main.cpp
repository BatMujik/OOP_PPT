#include "Shape.h"
#include "Slide.h"
#include "SlideShow.h"
#include "Tokenizer.h"
#include "CommandParser.h"
#include "ShapeFactory.h"
#include "Color.h"
#include "Canvas.h"
#include "UndoManager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <iomanip>

// ============== Display Mode ==============
enum DisplayMode {
    MODE_TEXT,
    MODE_VISUAL
};

static DisplayMode currentDisplayMode = MODE_TEXT;

// ============== Application Banner ==============
void displayBanner() {
    std::cout << Color::Cyan().toAnsiFg();
    std::cout << R"(
    ╔═════════════════════════════════════════════════════════════════════════╗  
    ║                                                                         ║  
    ║                                                                         ║    
    ║   ███████╗██╗     ██╗██████╗ ███████╗███████╗██╗  ██╗ ██████╗ ██╗    ██╗║
    ║   ██╔════╝██║     ██║██╔══██╗██╔════╝██╔════╝██║  ██║██╔═══██╗██║    ██║║
    ║   ███████╗██║     ██║██║  ██║█████╗  ███████╗███████║██║   ██║██║ █╗ ██║║
    ║   ╚════██║██║     ██║██║  ██║██╔══╝  ╚════██║██╔══██║██║   ██║██║███╗██║║
    ║   ███████║███████╗██║██████╔╝███████╗███████║██║  ██║╚██████╔╝╚███╔███╔╝║
    ║   ╚══════╝╚══════╝╚═╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝ ╚═════╝  ╚══╝╚══╝ ║
    ║                                                                         ║
    ║              )" << Color::Yellow().toAnsiFg() << "Advanced Visual Presentation System" << Color::Cyan().toAnsiFg() << R"(                        ║
    ║                       )" << Color::Gray().toAnsiFg() << "Version 2.0 - OOP Course Project" << Color::Cyan().toAnsiFg() << R"(                  ║
    ╚═════════════════════════════════════════════════════════════════════════╝
)" << Color::resetAnsi() << std::endl;
}

// ============== Help Display ==============
void displayHelp() {
    std::cout << Color::Cyan().toAnsiFg() << "\n╔═══════════════════════════════════════════════════════════════╗" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Yellow().toAnsiFg() << "                    AVAILABLE COMMANDS                         " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "╠═══════════════════════════════════════════════════════════════╣" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Green().toAnsiFg() << " NAVIGATION                                                    " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   next (n)          - Move to the next slide                  " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   prev (p)          - Move to the previous slide              " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   show (s)          - Display the current slide               " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   goto (g) <n>      - Jump to slide number n                  " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   goto <file> <n>   - Jump to slide n in another file         " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   nextfile (nf)     - Switch to next presentation             " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   prevfile (pf)     - Switch to previous presentation         " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "╠═══════════════════════════════════════════════════════════════╣" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Green().toAnsiFg() << " SHAPE MANIPULATION                                            " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   add (a)           - Add a new shape (interactive)           " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   delete (del) <n>  - Delete shape by index                   " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   edit (e) <n>      - Edit shape properties (interactive)     " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   list (ls)         - List all shapes on current slide        " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   clear             - Remove all shapes from current slide    " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "╠═══════════════════════════════════════════════════════════════╣" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Green().toAnsiFg() << " SLIDE MANIPULATION                                            " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   newslide (ns)     - Create a new empty slide                " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   delslide (ds) <n> - Delete slide by number                  " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "╠═══════════════════════════════════════════════════════════════╣" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Green().toAnsiFg() << " VIEW MODES                                                    " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   visual (v)        - Display slide as ASCII art graphic      " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   text (t)          - Display slide as text list              " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "╠═══════════════════════════════════════════════════════════════╣" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Green().toAnsiFg() << " EXPORT                                                        " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   export <file>     - Export current slide to BMP/SVG         " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   exportall <pre>   - Export all slides (prefix_1.bmp, etc)   " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   exportjson <file> - Export slide/slideshow to JSON          " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "╠═══════════════════════════════════════════════════════════════╣" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Green().toAnsiFg() << " FILE OPERATIONS                                               " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   new <file>       - Create new empty presentation            " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   save             - Save current presentation to file        " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   saveas <file>    - Save presentation to a new file          " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "╠═══════════════════════════════════════════════════════════════╣" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Green().toAnsiFg() << " UNDO/REDO                                                     " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   undo (u/z)        - Undo last action                        " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   redo (r/y)        - Redo previously undone action           " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   history           - Show undo/redo history status           " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "╠═══════════════════════════════════════════════════════════════╣" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::Green().toAnsiFg() << " UTILITIES                                                     " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   shapes            - Show available shape types              " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   colors            - Show available colors                   " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   help (h)          - Display this help message               " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << "   exit (q)          - Exit the program                        " << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Cyan().toAnsiFg() << "╚═══════════════════════════════════════════════════════════════╝" << Color::resetAnsi() << "\n" << std::endl;
}

// ============== Display Available Shapes ==============
void displayAvailableShapes() {
    std::vector<std::string> shapes = ShapeFactory::getAvailableShapes();
    
    std::cout << Color::Cyan().toAnsiFg() << "\n  ═══════════ Available Shapes ═══════════" << Color::resetAnsi() << std::endl;
    
    for (size_t i = 0; i < shapes.size(); ++i) {
        Color c = ShapeFactory::getDefaultColor(shapes[i]);
        
        // Create a temporary shape to get its icon
        ShapeFactory::ShapeParams params;
        params.x = 0;
        params.y = 0;
        params.fillColor = c;
        auto tempShape = ShapeFactory::create(shapes[i], params);
        
        std::string icon = tempShape ? tempShape->getAsciiIcon() : "?";
        
        std::cout << "  " << Color::Yellow().toAnsiFg() << std::setw(2) << (i + 1) << ". "
                  << Color::resetAnsi() << c.toAnsiFg() << icon << " "
                  << Color::resetAnsi() << std::setw(12) << std::left << shapes[i]
                  << c.toAnsiFg() << " ■" << Color::resetAnsi() << std::endl;
    }
    
    std::cout << Color::Cyan().toAnsiFg() << "  ═════════════════════════════════════════" << Color::resetAnsi() << "\n" << std::endl;
}

// ============== Display Available Colors ==============
void displayAvailableColors() {
    std::vector<std::string> colors = Color::getAvailableColors();
    
    std::cout << Color::Cyan().toAnsiFg() << "\n  ═══════════ Available Colors ═══════════" << Color::resetAnsi() << std::endl;
    
    for (size_t i = 0; i < colors.size(); ++i) {
        Color c(colors[i]);
        std::cout << "  " << Color::Yellow().toAnsiFg() << std::setw(2) << (i + 1) << ". "
                  << Color::resetAnsi() << c.toAnsiFg() << "████ " 
                  << Color::resetAnsi() << colors[i] << std::endl;
    }
    
    std::cout << Color::Cyan().toAnsiFg() << "  ═════════════════════════════════════════" << Color::resetAnsi() << "\n" << std::endl;
}

// ============== Load Presentation ==============
std::unique_ptr<SlideShow> loadPresentation(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                  << "Failed to open file: " << filename << std::endl;
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
        
        // Parse shape using factory
        std::vector<std::string> tokens = Tokenizer::split(trimmedLine, ',');
        if (tokens.size() >= 3) {
            auto shape = ShapeFactory::createFromLine(tokens);
            if (shape) {
                currentSlide.addShape(std::shared_ptr<Shape>(shape.release()));
                hasContent = true;
            } else {
                std::cerr << Color::Yellow().toAnsiFg() << "[WARN] " << Color::resetAnsi()
                          << "Unknown shape in " << filename << ": " << line << std::endl;
            }
        } else {
            std::cerr << Color::Yellow().toAnsiFg() << "[WARN] " << Color::resetAnsi()
                      << "Invalid line format in " << filename << ": " << line << std::endl;
        }
    }
    
    // Add the last slide if it has content
    if (hasContent || currentSlide.getShapeCount() > 0) {
        slideshow->addSlide(currentSlide);
    }

    file.close();
    
    if (slideshow->isEmpty()) {
        std::cerr << Color::Yellow().toAnsiFg() << "[WARN] " << Color::resetAnsi()
                  << "No slides found in: " << filename << std::endl;
    } else {
        std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                  << "Loaded " << slideshow->getTotalSlides() << " slide(s) from: " 
                  << Color::Yellow().toAnsiFg() << filename << Color::resetAnsi() << std::endl;
    }

    return slideshow;
}

// ============== Display Prompt ==============
void displayPrompt(const SlideShow* currentShow) {
    if (currentShow && !currentShow->isEmpty()) {
        std::cout << Color::Cyan().toAnsiFg() << "[" << Color::resetAnsi()
                  << Color::Yellow().toAnsiFg() << currentShow->getFilename() << Color::resetAnsi()
                  << Color::Cyan().toAnsiFg() << ", " << Color::resetAnsi()
                  << Color::Green().toAnsiFg() << "Slide " 
                  << currentShow->getCurrentSlideNumber() << "/" 
                  << currentShow->getTotalSlides() << Color::resetAnsi()
                  << Color::Cyan().toAnsiFg() << "]" << Color::resetAnsi()
                  << " " << Color::Magenta().toAnsiFg() << "▶ " << Color::resetAnsi();
    } else {
        std::cout << Color::Gray().toAnsiFg() << "[No presentation]" << Color::resetAnsi()
                  << " " << Color::Magenta().toAnsiFg() << "▶ " << Color::resetAnsi();
    }
}

// ============== Display Current Slide ==============
void displayCurrentSlide(SlideShow* slideshow) {
    if (currentDisplayMode == MODE_VISUAL) {
        slideshow->showVisual();
    } else {
        slideshow->show();
    }
}

// ============== Interactive Add Shape ==============
void interactiveAddShape(Slide* slide) {
    std::vector<std::string> shapes = ShapeFactory::getAvailableShapes();
    
    std::cout << Color::Cyan().toAnsiFg() << "\n  ═══════════ Add New Shape ═══════════" << Color::resetAnsi() << std::endl;
    
    for (size_t i = 0; i < shapes.size(); ++i) {
        Color c = ShapeFactory::getDefaultColor(shapes[i]);
        ShapeFactory::ShapeParams params;
        params.fillColor = c;
        auto tempShape = ShapeFactory::create(shapes[i], params);
        std::string icon = tempShape ? tempShape->getAsciiIcon() : "?";
        
        std::cout << "  " << Color::Yellow().toAnsiFg() << std::setw(2) << (i + 1) << ". "
                  << Color::resetAnsi() << c.toAnsiFg() << icon << " "
                  << Color::resetAnsi() << shapes[i] << std::endl;
    }
    std::cout << "  " << Color::Gray().toAnsiFg() << (shapes.size() + 1) << ". Cancel" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "  ═════════════════════════════════════════" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Yellow().toAnsiFg() << "  Enter shape number: " << Color::resetAnsi();
    std::string input;
    if (!std::getline(std::cin, input)) return;
    
    int selection;
    try {
        selection = std::stoi(Tokenizer::trim(input));
    } catch (...) {
        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Invalid selection." << std::endl;
        return;
    }
    
    if (selection == static_cast<int>(shapes.size() + 1) || selection < 1) {
        std::cout << Color::Gray().toAnsiFg() << "[INFO] " << Color::resetAnsi() << "Cancelled." << std::endl;
        return;
    }
    
    if (selection > static_cast<int>(shapes.size())) {
        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Invalid selection." << std::endl;
        return;
    }
    
    std::string shapeName = shapes[selection - 1];
    
    // Get position
    std::cout << Color::Yellow().toAnsiFg() << "  Enter position (x y): " << Color::resetAnsi();
    if (!std::getline(std::cin, input)) return;
    
    std::vector<std::string> posTokens = CommandParser::tokenize(input);
    if (posTokens.size() < 2) {
        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Please enter x and y coordinates." << std::endl;
        return;
    }
    
    int x, y;
    try {
        x = std::stoi(posTokens[0]);
        y = std::stoi(posTokens[1]);
    } catch (...) {
        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Invalid coordinates." << std::endl;
        return;
    }
    
    // Get size (optional)
    std::cout << Color::Yellow().toAnsiFg() << "  Enter size (width height) or press Enter for default: " << Color::resetAnsi();
    if (!std::getline(std::cin, input)) return;
    
    int width = 40, height = 30;
    std::vector<std::string> sizeTokens = CommandParser::tokenize(input);
    if (sizeTokens.size() >= 2) {
        try {
            width = std::stoi(sizeTokens[0]);
            height = std::stoi(sizeTokens[1]);
        } catch (...) {
            // Use defaults
        }
    } else if (sizeTokens.size() == 1) {
        try {
            width = height = std::stoi(sizeTokens[0]);
        } catch (...) {}
    }
    
    // Get color (with picker)
    std::vector<std::string> colors = Color::getAvailableColors();
    std::cout << Color::Cyan().toAnsiFg() << "\n  ═══════════ Choose Color ═══════════" << Color::resetAnsi() << std::endl;
    
    for (size_t i = 0; i < colors.size(); ++i) {
        Color c(colors[i]);
        std::cout << "  " << Color::Yellow().toAnsiFg() << std::setw(2) << (i + 1) << ". "
                  << Color::resetAnsi() << c.toAnsiFg() << "████ " 
                  << Color::resetAnsi() << colors[i] << std::endl;
    }
    std::cout << "  " << Color::Gray().toAnsiFg() << (colors.size() + 1) << ". Default (" 
              << ShapeFactory::getDefaultColor(shapeName).getName() << ")" << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "  ═════════════════════════════════════════" << Color::resetAnsi() << std::endl;
    
    std::cout << Color::Yellow().toAnsiFg() << "  Enter color number or name: " << Color::resetAnsi();
    if (!std::getline(std::cin, input)) return;
    
    Color fillColor = ShapeFactory::getDefaultColor(shapeName);
    std::string colorInput = Tokenizer::trim(input);
    if (!colorInput.empty()) {
        // Check if it's a number
        try {
            int colorIdx = std::stoi(colorInput);
            if (colorIdx >= 1 && colorIdx <= static_cast<int>(colors.size())) {
                fillColor = Color(colors[colorIdx - 1]);
            }
            // else use default
        } catch (...) {
            // Not a number, treat as color name
            fillColor = Color(colorInput);
        }
    }
    
    // Create the shape
    ShapeFactory::ShapeParams params;
    params.x = x;
    params.y = y;
    params.width = width;
    params.height = height;
    params.fillColor = fillColor;
    
    auto shape = ShapeFactory::create(shapeName, params);
    if (shape) {
        slide->addShape(std::shared_ptr<Shape>(shape.release()));
        std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                  << "Added " << fillColor.toAnsiFg() << shapeName << Color::resetAnsi()
                  << " at (" << x << ", " << y << ")" << std::endl;
    } else {
        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Failed to create shape." << std::endl;
    }
}

// ============== Interactive Edit Shape ==============
void interactiveEditShape(Slide* slide, size_t index) {
    auto shape = slide->getShape(index);
    if (!shape) {
        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Invalid shape index." << std::endl;
        return;
    }
    
    std::cout << Color::Cyan().toAnsiFg() << "\n  ═══════════ Edit " << shape->getName() << " ═══════════" << Color::resetAnsi() << std::endl;
    std::cout << "  Current: (" << shape->getX() << ", " << shape->getY() << ") "
              << "[" << shape->getWidth() << "x" << shape->getHeight() << "] "
              << shape->getFillColor().getName() << std::endl;
    
    std::cout << Color::Yellow().toAnsiFg() << "\n  1. " << Color::resetAnsi() << "Change position" << std::endl;
    std::cout << Color::Yellow().toAnsiFg() << "  2. " << Color::resetAnsi() << "Change size" << std::endl;
    std::cout << Color::Yellow().toAnsiFg() << "  3. " << Color::resetAnsi() << "Change color" << std::endl;
    std::cout << Color::Yellow().toAnsiFg() << "  4. " << Color::resetAnsi() << "Cancel" << std::endl;
    
    std::cout << Color::Yellow().toAnsiFg() << "\n  Enter option: " << Color::resetAnsi();
    std::string input;
    if (!std::getline(std::cin, input)) return;
    
    int option;
    try {
        option = std::stoi(Tokenizer::trim(input));
    } catch (...) {
        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Invalid option." << std::endl;
        return;
    }
    
    switch (option) {
        case 1: {
            std::cout << Color::Yellow().toAnsiFg() << "  New position (x y): " << Color::resetAnsi();
            if (!std::getline(std::cin, input)) return;
            std::vector<std::string> tokens = CommandParser::tokenize(input);
            if (tokens.size() >= 2) {
                try {
                    shape->setPosition(std::stoi(tokens[0]), std::stoi(tokens[1]));
                    std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi() << "Position updated." << std::endl;
                } catch (...) {
                    std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Invalid coordinates." << std::endl;
                }
            }
            break;
        }
        case 2: {
            std::cout << Color::Yellow().toAnsiFg() << "  New size (width height): " << Color::resetAnsi();
            if (!std::getline(std::cin, input)) return;
            std::vector<std::string> tokens = CommandParser::tokenize(input);
            if (tokens.size() >= 2) {
                try {
                    shape->setSize(std::stoi(tokens[0]), std::stoi(tokens[1]));
                    std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi() << "Size updated." << std::endl;
                } catch (...) {
                    std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi() << "Invalid size." << std::endl;
                }
            }
            break;
        }
        case 3: {
            std::cout << Color::Yellow().toAnsiFg() << "  New color name: " << Color::resetAnsi();
            if (!std::getline(std::cin, input)) return;
            input = Tokenizer::trim(input);
            if (!input.empty()) {
                shape->setFillColor(Color(input));
                std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi() << "Color updated to "
                          << shape->getFillColor().toAnsiFg() << shape->getFillColor().getName() 
                          << Color::resetAnsi() << std::endl;
            }
            break;
        }
        default:
            std::cout << Color::Gray().toAnsiFg() << "[INFO] " << Color::resetAnsi() << "Cancelled." << std::endl;
    }
}

// ============== Main Entry Point ==============
int main(int argc, char* argv[]) {
    // Enable Windows console for ANSI colors
    #ifdef _WIN32
    system("chcp 65001 > nul");  // UTF-8
    #endif
    
    displayBanner();
    
    if (argc < 2) {
        std::cerr << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                  << "Usage: " << argv[0] << " <presentation1.txt> [presentation2.txt] ..." << std::endl;
        std::cerr << Color::Gray().toAnsiFg() << "       Creating empty presentation..." << Color::resetAnsi() << std::endl;
    }

    // Load all presentations
    std::vector<std::unique_ptr<SlideShow>> slideshows;
    std::map<std::string, int> filenameToIndex;

    std::cout << Color::Cyan().toAnsiFg() << "\n[INFO] " << Color::resetAnsi() << "Loading presentations..." << std::endl;
    
    for (int i = 1; i < argc; ++i) {
        std::string filename = argv[i];
        auto slideshow = loadPresentation(filename);
        if (slideshow && !slideshow->isEmpty()) {
            std::string normalizedName = CommandParser::normalizePath(filename);
            filenameToIndex[normalizedName] = static_cast<int>(slideshows.size());
            slideshows.push_back(std::move(slideshow));
        }
    }

    // Create empty slideshow if none loaded
    if (slideshows.empty()) {
        auto emptyShow = std::make_unique<SlideShow>("untitled.txt");
        emptyShow->addSlide(Slide());
        slideshows.push_back(std::move(emptyShow));
        std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                  << "Created empty presentation." << std::endl;
    }

    int currentShowIndex = 0;
    
    // Initialize undo manager for each slideshow
    std::vector<UndoManager> undoManagers(slideshows.size());
    
    std::cout << Color::Green().toAnsiFg() << "\n[OK] " << Color::resetAnsi()
              << slideshows.size() << " presentation(s) loaded. Type "
              << Color::Yellow().toAnsiFg() << "help" << Color::resetAnsi() 
              << " for commands.\n" << std::endl;

    // Display initial slide
    displayCurrentSlide(slideshows[currentShowIndex].get());

    // Main CLI loop
    std::string input;
    bool running = true;

    while (running) {
        displayPrompt(slideshows[currentShowIndex].get());
        
        if (!std::getline(std::cin, input)) {
            break;
        }

        CommandParser::Command cmd = CommandParser::parse(input);

        switch (cmd.type) {
            case CommandParser::NEXT:
                if (slideshows[currentShowIndex]->next()) {
                    displayCurrentSlide(slideshows[currentShowIndex].get());
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Already at the last slide." << std::endl;
                }
                break;

            case CommandParser::PREV:
                if (slideshows[currentShowIndex]->prev()) {
                    displayCurrentSlide(slideshows[currentShowIndex].get());
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Already at the first slide." << std::endl;
                }
                break;

            case CommandParser::SHOW:
                displayCurrentSlide(slideshows[currentShowIndex].get());
                break;

            case CommandParser::GOTO:
                if (!cmd.args.empty()) {
                try {
                    int slideNum = std::stoi(cmd.args[0]);
                    if (slideshows[currentShowIndex]->gotoSlide(slideNum)) {
                            displayCurrentSlide(slideshows[currentShowIndex].get());
                    } else {
                            std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                      << "Invalid slide number. Valid range: 1-" 
                                  << slideshows[currentShowIndex]->getTotalSlides() << std::endl;
                        }
                    } catch (...) {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Invalid slide number format." << std::endl;
                    }
                }
                break;

            case CommandParser::GOTO_FILE:
                if (cmd.args.size() >= 2) {
                    std::string targetFile = cmd.args[0];
                    auto it = filenameToIndex.find(targetFile);
                    
                    if (it != filenameToIndex.end()) {
                        try {
                            int slideNum = std::stoi(cmd.args[1]);
                            int targetIndex = it->second;
                            
                            if (slideshows[targetIndex]->gotoSlide(slideNum)) {
                                currentShowIndex = targetIndex;
                                displayCurrentSlide(slideshows[currentShowIndex].get());
                            } else {
                                std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                          << "Invalid slide number for " << targetFile 
                                          << ". Valid range: 1-" << slideshows[targetIndex]->getTotalSlides() << std::endl;
                            }
                        } catch (...) {
                            std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                      << "Invalid slide number format." << std::endl;
                        }
                    } else {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Presentation not found: " << targetFile << std::endl;
                        std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                                  << "Available presentations:" << std::endl;
                        for (const auto& pair : filenameToIndex) {
                            std::cout << "  - " << pair.first << std::endl;
                        }
                    }
                }
                break;

            case CommandParser::NEXTFILE:
                if (currentShowIndex < static_cast<int>(slideshows.size()) - 1) {
                    currentShowIndex++;
                    std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                              << "Switched to: " << Color::Yellow().toAnsiFg()
                              << slideshows[currentShowIndex]->getFilename() << Color::resetAnsi() << std::endl;
                    displayCurrentSlide(slideshows[currentShowIndex].get());
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Already at the last presentation." << std::endl;
                }
                break;

            case CommandParser::PREVFILE:
                if (currentShowIndex > 0) {
                    currentShowIndex--;
                    std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                              << "Switched to: " << Color::Yellow().toAnsiFg()
                              << slideshows[currentShowIndex]->getFilename() << Color::resetAnsi() << std::endl;
                    displayCurrentSlide(slideshows[currentShowIndex].get());
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Already at the first presentation." << std::endl;
                }
                break;

            case CommandParser::ADD:
                {
                    Slide* currentSlide = slideshows[currentShowIndex]->getCurrentSlide();
                    if (currentSlide) {
                        // Save state for undo before adding
                        undoManagers[currentShowIndex].saveState(
                            slideshows[currentShowIndex]->getCurrentSlideNumber(),
                            *currentSlide,
                            "Add shape"
                        );
                        interactiveAddShape(currentSlide);
                        slideshows[currentShowIndex]->setModified(true);
                    } else {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "No current slide." << std::endl;
                    }
                }
                break;

            case CommandParser::DELETE:
                if (!cmd.args.empty()) {
                    try {
                        int index = std::stoi(cmd.args[0]) - 1;
                        Slide* currentSlide = slideshows[currentShowIndex]->getCurrentSlide();
                        if (currentSlide) {
                            // Save state for undo before deleting
                            undoManagers[currentShowIndex].saveState(
                                slideshows[currentShowIndex]->getCurrentSlideNumber(),
                                *currentSlide,
                                "Delete shape"
                            );
                            if (currentSlide->removeShape(index)) {
                                std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                          << "Shape deleted." << std::endl;
                                slideshows[currentShowIndex]->setModified(true);
                            } else {
                                std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                          << "Invalid shape index." << std::endl;
                            }
                        }
                    } catch (...) {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Invalid index format." << std::endl;
                    }
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Usage: delete <shape_number>" << std::endl;
                }
                break;

            case CommandParser::EDIT:
                if (!cmd.args.empty()) {
                    try {
                        int index = std::stoi(cmd.args[0]) - 1;
                        Slide* currentSlide = slideshows[currentShowIndex]->getCurrentSlide();
                        if (currentSlide) {
                            // Save state for undo before editing
                            undoManagers[currentShowIndex].saveState(
                                slideshows[currentShowIndex]->getCurrentSlideNumber(),
                                *currentSlide,
                                "Edit shape"
                            );
                            interactiveEditShape(currentSlide, index);
                            slideshows[currentShowIndex]->setModified(true);
                        }
                    } catch (...) {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Invalid index format." << std::endl;
                    }
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Usage: edit <shape_number>" << std::endl;
                }
                break;

            case CommandParser::LIST:
                {
                    Slide* currentSlide = slideshows[currentShowIndex]->getCurrentSlide();
                    if (currentSlide) {
                        currentSlide->listShapes();
                    }
                }
                        break;

            case CommandParser::CLEAR:
                {
                    Slide* currentSlide = slideshows[currentShowIndex]->getCurrentSlide();
                    if (currentSlide) {
                        // Save state for undo before clearing
                        undoManagers[currentShowIndex].saveState(
                            slideshows[currentShowIndex]->getCurrentSlideNumber(),
                            *currentSlide,
                            "Clear all shapes"
                        );
                        while (currentSlide->getShapeCount() > 0) {
                            currentSlide->removeShape(0);
                        }
                        std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                  << "All shapes cleared from current slide." << std::endl;
                        slideshows[currentShowIndex]->setModified(true);
                    }
                }
                break;

            case CommandParser::NEWSLIDE:
                {
                    int newIndex = slideshows[currentShowIndex]->addNewSlide();
                    slideshows[currentShowIndex]->gotoSlide(newIndex);
                    std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                              << "Created new slide " << newIndex << "." << std::endl;
                    displayCurrentSlide(slideshows[currentShowIndex].get());
                }
                break;

            case CommandParser::DELSLIDE:
                if (!cmd.args.empty()) {
                    try {
                        int slideNum = std::stoi(cmd.args[0]);
                        if (slideshows[currentShowIndex]->removeSlide(slideNum)) {
                            std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                      << "Deleted slide " << slideNum << "." << std::endl;
                            displayCurrentSlide(slideshows[currentShowIndex].get());
                        } else {
                            std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                      << "Invalid slide number." << std::endl;
                        }
                    } catch (...) {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Invalid slide number format." << std::endl;
                    }
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Usage: delslide <slide_number>" << std::endl;
                }
                break;

            case CommandParser::VISUAL:
                currentDisplayMode = MODE_VISUAL;
                std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                          << "Switched to visual mode." << std::endl;
                displayCurrentSlide(slideshows[currentShowIndex].get());
                break;

            case CommandParser::TEXT:
                currentDisplayMode = MODE_TEXT;
                std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                          << "Switched to text mode." << std::endl;
                displayCurrentSlide(slideshows[currentShowIndex].get());
                break;

            case CommandParser::EXPORT:
                if (!cmd.args.empty()) {
                    std::string filename = cmd.args[0];
                    bool success = false;
                    
                    // Determine format from extension
                    if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".svg") {
                        success = slideshows[currentShowIndex]->exportCurrentSlideSVG(filename);
                    } else {
                        // Default to BMP
                        if (filename.size() <= 4 || filename.substr(filename.size() - 4) != ".bmp") {
                            filename += ".bmp";
                        }
                        success = slideshows[currentShowIndex]->exportCurrentSlideBMP(filename);
                    }
                    
                    if (success) {
                        std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                  << "Exported to: " << Color::Yellow().toAnsiFg() << filename 
                                  << Color::resetAnsi() << std::endl;
                    } else {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Failed to export." << std::endl;
                    }
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Usage: export <filename.bmp|filename.svg>" << std::endl;
                }
                break;

            case CommandParser::EXPORT_ALL:
                {
                    std::string prefix = cmd.args.empty() ? "slide" : cmd.args[0];
                    std::cout << Color::Cyan().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Exporting all slides as BMP..." << std::endl;
                    slideshows[currentShowIndex]->exportAllSlidesBMP(prefix);
                    std::cout << Color::Cyan().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Exporting all slides as SVG..." << std::endl;
                    slideshows[currentShowIndex]->exportAllSlidesSVG(prefix);
                }
                break;

            case CommandParser::EXPORT_JSON:
                if (!cmd.args.empty()) {
                    std::string filename = cmd.args[0];
                    bool success = false;
                    
                    // Add .json extension if not present
                    if (filename.size() <= 5 || filename.substr(filename.size() - 5) != ".json") {
                        filename += ".json";
                    }
                    
                    // Check if user wants to export entire slideshow (with "all" keyword)
                    if (cmd.args.size() >= 2 && (cmd.args[1] == "all" || cmd.args[1] == "-a")) {
                        success = slideshows[currentShowIndex]->exportSlideshowJSON(filename);
                        if (success) {
                            std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                      << "Exported entire slideshow to: " << Color::Yellow().toAnsiFg() 
                                      << filename << Color::resetAnsi() << std::endl;
                        }
                    } else {
                        success = slideshows[currentShowIndex]->exportCurrentSlideJSON(filename);
                        if (success) {
                            std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                      << "Exported current slide to: " << Color::Yellow().toAnsiFg() 
                                      << filename << Color::resetAnsi() << std::endl;
                        }
                    }
                    
                    if (!success) {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Failed to export JSON." << std::endl;
                    }
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Usage: exportjson <filename.json> [all]" << std::endl;
                    std::cout << Color::Gray().toAnsiFg() << "       Add 'all' to export entire slideshow" 
                              << Color::resetAnsi() << std::endl;
                }
                break;
                            
            case CommandParser::SHAPES:
                displayAvailableShapes();
                                break;

            case CommandParser::COLORS:
                displayAvailableColors();
                break;

            case CommandParser::NEW:
                {
                    std::string newFilename = "new_presentation.txt";
                    if (!cmd.args.empty()) {
                        newFilename = cmd.args[0];
                        if (newFilename.find('.') == std::string::npos) {
                            newFilename += ".txt";
                        }
                    }
                    
                    // Create new empty slideshow
                    auto newShow = std::make_unique<SlideShow>(newFilename);
                    newShow->addSlide(Slide());  // Add one empty slide
                    
                    // Add to index
                    std::string normalizedName = CommandParser::normalizePath(newFilename);
                    filenameToIndex[normalizedName] = static_cast<int>(slideshows.size());
                    
                    slideshows.push_back(std::move(newShow));
                    undoManagers.push_back(UndoManager());  // Add undo manager for new slideshow
                    currentShowIndex = static_cast<int>(slideshows.size()) - 1;
                    
                    std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                              << "Created new presentation: " << Color::Yellow().toAnsiFg()
                              << newFilename << Color::resetAnsi() << std::endl;
                    std::cout << Color::Gray().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Use 'add' to add shapes, then 'save' to save the file." << std::endl;
                    
                    displayCurrentSlide(slideshows[currentShowIndex].get());
                }
                break;

            case CommandParser::SAVE:
                {
                    if (slideshows[currentShowIndex]->save()) {
                        std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                  << "Saved to: " << Color::Yellow().toAnsiFg()
                                  << slideshows[currentShowIndex]->getFilename() 
                                  << Color::resetAnsi() << std::endl;
                    } else {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Failed to save file." << std::endl;
                    }
                }
                break;

            case CommandParser::SAVEAS:
                if (!cmd.args.empty()) {
                    std::string newFilename = cmd.args[0];
                    // Add .txt extension if not present
                    if (newFilename.find('.') == std::string::npos) {
                        newFilename += ".txt";
                    }
                    
                    if (slideshows[currentShowIndex]->save(newFilename)) {
                        // Update the filename index map
                        std::string normalizedName = CommandParser::normalizePath(newFilename);
                        filenameToIndex[normalizedName] = currentShowIndex;
                        
                        std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                  << "Saved to: " << Color::Yellow().toAnsiFg()
                                  << newFilename << Color::resetAnsi() << std::endl;
                    } else {
                        std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                                  << "Failed to save file." << std::endl;
                    }
                } else {
                    std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                              << "Usage: saveas <filename.txt>" << std::endl;
                }
                break;

            case CommandParser::UNDO:
                {
                    Slide* currentSlide = slideshows[currentShowIndex]->getCurrentSlide();
                    if (currentSlide && undoManagers[currentShowIndex].canUndo()) {
                        SlideSnapshot* snapshot = undoManagers[currentShowIndex].undo(*currentSlide);
                        if (snapshot) {
                            // Navigate to the slide that was modified
                            slideshows[currentShowIndex]->gotoSlide(snapshot->slideIndex);
                            // Restore the slide state
                            Slide* targetSlide = slideshows[currentShowIndex]->getCurrentSlide();
                            if (targetSlide) {
                                *targetSlide = snapshot->slideState;
                            }
                            std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                      << "Undone: " << Color::Yellow().toAnsiFg() 
                                      << snapshot->description << Color::resetAnsi() << std::endl;
                            displayCurrentSlide(slideshows[currentShowIndex].get());
                        }
                    } else {
                        std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                                  << "Nothing to undo." << std::endl;
                    }
                }
                break;

            case CommandParser::REDO:
                {
                    Slide* currentSlide = slideshows[currentShowIndex]->getCurrentSlide();
                    if (currentSlide && undoManagers[currentShowIndex].canRedo()) {
                        SlideSnapshot* snapshot = undoManagers[currentShowIndex].redo(*currentSlide);
                        if (snapshot) {
                            // Navigate to the slide that was modified
                            slideshows[currentShowIndex]->gotoSlide(snapshot->slideIndex);
                            // Restore the slide state
                            Slide* targetSlide = slideshows[currentShowIndex]->getCurrentSlide();
                            if (targetSlide) {
                                *targetSlide = snapshot->slideState;
                            }
                            std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                                      << "Redone: " << Color::Yellow().toAnsiFg() 
                                      << snapshot->description << Color::resetAnsi() << std::endl;
                            displayCurrentSlide(slideshows[currentShowIndex].get());
                        }
                    } else {
                        std::cout << Color::Yellow().toAnsiFg() << "[INFO] " << Color::resetAnsi()
                                  << "Nothing to redo." << std::endl;
                    }
                }
                break;

            case CommandParser::HISTORY:
                {
                    std::cout << Color::Cyan().toAnsiFg() << "\n  ═══════════ Undo/Redo History ═══════════" 
                              << Color::resetAnsi() << std::endl;
                    
                    size_t undoCount = undoManagers[currentShowIndex].getUndoCount();
                    size_t redoCount = undoManagers[currentShowIndex].getRedoCount();
                    
                    std::cout << "  " << Color::Yellow().toAnsiFg() << "Undo stack: " << Color::resetAnsi()
                              << undoCount << " action(s)" << std::endl;
                    if (undoCount > 0) {
                        std::cout << "    Next undo: " << Color::Gray().toAnsiFg() 
                                  << undoManagers[currentShowIndex].getUndoDescription() 
                                  << Color::resetAnsi() << std::endl;
                    }
                    
                    std::cout << "  " << Color::Yellow().toAnsiFg() << "Redo stack: " << Color::resetAnsi()
                              << redoCount << " action(s)" << std::endl;
                    if (redoCount > 0) {
                        std::cout << "    Next redo: " << Color::Gray().toAnsiFg() 
                                  << undoManagers[currentShowIndex].getRedoDescription() 
                                  << Color::resetAnsi() << std::endl;
                    }
                    
                    std::cout << Color::Cyan().toAnsiFg() << "  ═════════════════════════════════════════" 
                              << Color::resetAnsi() << "\n" << std::endl;
                }
                break;

            case CommandParser::HELP:
                displayHelp();
                break;

            case CommandParser::EXIT:
                std::cout << Color::Cyan().toAnsiFg() << "\n[INFO] " << Color::resetAnsi()
                          << "Exiting slideshow. " << Color::Yellow().toAnsiFg() 
                          << "Goodbye!" << Color::resetAnsi() << "\n" << std::endl;
                running = false;
                break;

            case CommandParser::INVALID:
                if (!Tokenizer::trim(input).empty()) {
                    std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                              << "Unknown command. Type " << Color::Yellow().toAnsiFg() 
                              << "help" << Color::resetAnsi() << " for available commands." << std::endl;
                }
                break;

            default:
                break;
        }
    }

    return 0;
}
