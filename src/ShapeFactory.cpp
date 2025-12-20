#include "ShapeFactory.h"
#include "Tokenizer.h"
#include <algorithm>
#include <random>
#include <ctime>

const std::map<std::string, ShapeFactory::CreatorFunc>& ShapeFactory::getCreatorMap() {
    static const std::map<std::string, CreatorFunc> creators = {
        {"circle", [](const ShapeParams& p) {
            return std::make_unique<Circle>(p.x, p.y, p.width / 2, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"rectangle", [](const ShapeParams& p) {
            return std::make_unique<Rectangle>(p.x, p.y, p.width, p.height, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"square", [](const ShapeParams& p) {
            return std::make_unique<Square>(p.x, p.y, p.width, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"triangle", [](const ShapeParams& p) {
            return std::make_unique<Triangle>(p.x, p.y, p.width, p.height, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"diamond", [](const ShapeParams& p) {
            return std::make_unique<Diamond>(p.x, p.y, p.width, p.height, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"star", [](const ShapeParams& p) {
            return std::make_unique<Star>(p.x, p.y, p.width, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"hexagon", [](const ShapeParams& p) {
            return std::make_unique<Hexagon>(p.x, p.y, p.width, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"pentagon", [](const ShapeParams& p) {
            return std::make_unique<Pentagon>(p.x, p.y, p.width, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"oval", [](const ShapeParams& p) {
            return std::make_unique<Oval>(p.x, p.y, p.width, p.height, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"trapezoid", [](const ShapeParams& p) {
            return std::make_unique<Trapezoid>(p.x, p.y, p.width, p.height, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"rhombus", [](const ShapeParams& p) {
            return std::make_unique<Rhombus>(p.x, p.y, p.width, p.height, p.fillColor, p.borderColor, p.borderWidth, p.filled);
        }},
        {"line", [](const ShapeParams& p) {
            return std::make_unique<Line>(p.x, p.y, p.x2, p.y2, p.fillColor, p.borderWidth);
        }},
        {"text", [](const ShapeParams& p) {
            return std::make_unique<Text>(p.x, p.y, p.text, p.fillColor, 16);
        }}
    };
    return creators;
}

std::unique_ptr<Shape> ShapeFactory::create(const std::string& shapeName, const ShapeParams& params) {
    std::string lower = Tokenizer::toLower(shapeName);
    
    const auto& creators = getCreatorMap();
    auto it = creators.find(lower);
    
    if (it != creators.end()) {
        return it->second(params);
    }
    
    return nullptr;
}

std::unique_ptr<Shape> ShapeFactory::createSimple(const std::string& shapeName, int x, int y) {
    ShapeParams params;
    params.x = x;
    params.y = y;
    params.fillColor = getDefaultColor(shapeName);
    
    return create(shapeName, params);
}

std::unique_ptr<Shape> ShapeFactory::createFromLine(const std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return nullptr;
    }
    
    ShapeParams params;
    std::string shapeName = tokens[0];
    
    try {
        params.x = std::stoi(tokens[1]);
        params.y = std::stoi(tokens[2]);
        
        // Extended format: ShapeName, x, y, width, height, fillColor, borderColor, borderWidth, filled
        if (tokens.size() >= 4) {
            params.width = std::stoi(tokens[3]);
        }
        if (tokens.size() >= 5) {
            params.height = std::stoi(tokens[4]);
        }
        if (tokens.size() >= 6) {
            params.fillColor = Color(tokens[5]);
        } else {
            params.fillColor = getDefaultColor(shapeName);
        }
        if (tokens.size() >= 7) {
            params.borderColor = Color(tokens[6]);
        }
        if (tokens.size() >= 8) {
            params.borderWidth = std::stoi(tokens[7]);
        }
        if (tokens.size() >= 9) {
            std::string filledStr = Tokenizer::toLower(tokens[8]);
            params.filled = (filledStr == "true" || filledStr == "1" || filledStr == "yes");
        }
        
        // Special handling for text shapes
        if (Tokenizer::toLower(shapeName) == "text" && tokens.size() >= 4) {
            params.text = tokens[3];
        }
        
        // Special handling for line shapes (x, y, x2, y2)
        if (Tokenizer::toLower(shapeName) == "line" && tokens.size() >= 5) {
            params.x2 = std::stoi(tokens[3]);
            params.y2 = std::stoi(tokens[4]);
        }
        
    } catch (const std::exception&) {
        // Use defaults if parsing fails
        params.fillColor = getDefaultColor(shapeName);
    }
    
    return create(shapeName, params);
}

std::vector<std::string> ShapeFactory::getAvailableShapes() {
    return {
        "Circle", "Rectangle", "Square", "Triangle", "Diamond",
        "Star", "Hexagon", "Pentagon", "Oval", "Trapezoid", "Rhombus",
        "Line", "Text"
    };
}

bool ShapeFactory::isValidShape(const std::string& shapeName) {
    std::string lower = Tokenizer::toLower(shapeName);
    const auto& creators = getCreatorMap();
    return creators.find(lower) != creators.end();
}

Color ShapeFactory::getRandomColor() {
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::vector<Color> colors = {
        Color::Red(), Color::Green(), Color::Blue(), Color::Yellow(),
        Color::Cyan(), Color::Magenta(), Color::Orange(), Color::Purple(),
        Color::Pink(), Color::Gold(), Color::Teal(), Color::Lime(),
        Color::Coral(), Color::Violet()
    };
    std::uniform_int_distribution<int> dist(0, static_cast<int>(colors.size()) - 1);
    return colors[dist(rng)];
}

Color ShapeFactory::getDefaultColor(const std::string& shapeName) {
    std::string lower = Tokenizer::toLower(shapeName);
    
    static const std::map<std::string, Color> defaultColors = {
        {"circle", Color::Red()},
        {"rectangle", Color::Blue()},
        {"square", Color::Green()},
        {"triangle", Color::Yellow()},
        {"diamond", Color::Cyan()},
        {"star", Color::Gold()},
        {"hexagon", Color::Purple()},
        {"pentagon", Color::Teal()},
        {"oval", Color::Pink()},
        {"trapezoid", Color::Coral()},
        {"rhombus", Color::Violet()},
        {"line", Color::White()},
        {"text", Color::White()}
    };
    
    auto it = defaultColors.find(lower);
    if (it != defaultColors.end()) {
        return it->second;
    }
    return Color::White();
}

