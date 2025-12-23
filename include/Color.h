#ifndef COLOR_H
#define COLOR_H

#include <string>
#include <map>
#include <cstdint>
#include <vector>

/**
 * @brief Represents an RGB color with ANSI terminal support
 */
class Color {
public:
    uint8_t r, g, b;

    // Default constructor - white
    Color() : r(255), g(255), b(255) {}
    
    // RGB constructor
    Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

    // Named color constructor
    explicit Color(const std::string& colorName);

    /**
     * @brief Gets ANSI escape code for foreground color
     */
    std::string toAnsiFg() const;

    /**
     * @brief Gets ANSI escape code for background color
     */
    std::string toAnsiBg() const;

    /**
     * @brief Resets ANSI color
     */
    static std::string resetAnsi();

    /**
     * @brief Gets color name if it's a known color
     */
    std::string getName() const;

    /**
     * @brief Checks if two colors are equal
     */
    bool operator==(const Color& other) const;

    // Predefined colors
    static Color Red()     { return Color(255, 60, 60); }
    static Color Green()   { return Color(60, 255, 60); }
    static Color Blue()    { return Color(60, 120, 255); }
    static Color Yellow()  { return Color(255, 255, 60); }
    static Color Cyan()    { return Color(60, 255, 255); }
    static Color Magenta() { return Color(255, 60, 255); }
    static Color Orange()  { return Color(255, 165, 0); }
    static Color Purple()  { return Color(160, 60, 255); }
    static Color Pink()    { return Color(255, 150, 200); }
    static Color White()   { return Color(255, 255, 255); }
    static Color Black()   { return Color(0, 0, 0); }
    static Color Gray()    { return Color(128, 128, 128); }
    static Color Brown()   { return Color(139, 90, 43); }
    static Color Gold()    { return Color(255, 215, 0); }
    static Color Silver()  { return Color(192, 192, 192); }
    static Color Navy()    { return Color(30, 60, 150); }
    static Color Teal()    { return Color(60, 180, 180); }
    static Color Lime()    { return Color(180, 255, 60); }
    static Color Coral()   { return Color(255, 127, 80); }
    static Color Violet()  { return Color(200, 130, 255); }

    /**
     * @brief Gets list of all available color names
     */
    static std::vector<std::string> getAvailableColors();

private:
    static const std::map<std::string, Color>& getColorMap();
};

#endif // COLOR_H

