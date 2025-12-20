#include "Color.h"
#include <algorithm>
#include <cctype>
#include <sstream>

const std::map<std::string, Color>& Color::getColorMap() {
    static const std::map<std::string, Color> colors = {
        {"red", Red()},
        {"green", Green()},
        {"blue", Blue()},
        {"yellow", Yellow()},
        {"cyan", Cyan()},
        {"magenta", Magenta()},
        {"orange", Orange()},
        {"purple", Purple()},
        {"pink", Pink()},
        {"white", White()},
        {"black", Black()},
        {"gray", Gray()},
        {"grey", Gray()},
        {"brown", Brown()},
        {"gold", Gold()},
        {"silver", Silver()},
        {"navy", Navy()},
        {"teal", Teal()},
        {"lime", Lime()},
        {"coral", Coral()},
        {"violet", Violet()}
    };
    return colors;
}

Color::Color(const std::string& colorName) {
    std::string lower = colorName;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    const auto& colors = getColorMap();
    auto it = colors.find(lower);
    if (it != colors.end()) {
        *this = it->second;
    } else {
        // Default to white if color not found
        *this = White();
    }
}

std::string Color::toAnsiFg() const {
    std::ostringstream oss;
    oss << "\033[38;2;" << (int)r << ";" << (int)g << ";" << (int)b << "m";
    return oss.str();
}

std::string Color::toAnsiBg() const {
    std::ostringstream oss;
    oss << "\033[48;2;" << (int)r << ";" << (int)g << ";" << (int)b << "m";
    return oss.str();
}

std::string Color::resetAnsi() {
    return "\033[0m";
}

std::string Color::getName() const {
    const auto& colors = getColorMap();
    for (const auto& pair : colors) {
        if (pair.second == *this) {
            std::string name = pair.first;
            if (!name.empty()) {
                name[0] = std::toupper(name[0]);
            }
            return name;
        }
    }
    // Return RGB format if not a named color
    std::ostringstream oss;
    oss << "RGB(" << (int)r << "," << (int)g << "," << (int)b << ")";
    return oss.str();
}

bool Color::operator==(const Color& other) const {
    return r == other.r && g == other.g && b == other.b;
}

std::vector<std::string> Color::getAvailableColors() {
    return {
        "Red", "Green", "Blue", "Yellow", "Cyan", "Magenta",
        "Orange", "Purple", "Pink", "White", "Black", "Gray",
        "Brown", "Gold", "Silver", "Navy", "Teal", "Lime",
        "Coral", "Violet"
    };
}

