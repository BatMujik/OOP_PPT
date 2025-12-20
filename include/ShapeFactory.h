#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <memory>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include "Shape.h"
#include "Color.h"

/**
 * @brief Factory class for creating shapes (Factory Design Pattern)
 * Demonstrates the Factory pattern in OOP
 */
class ShapeFactory {
public:
    /**
     * @brief Shape creation parameters
     */
    struct ShapeParams {
        int x = 0;
        int y = 0;
        int width = 40;
        int height = 30;
        Color fillColor = Color::Blue();
        Color borderColor = Color::White();
        int borderWidth = 2;
        bool filled = true;
        std::string text = "";  // For Text shapes
        int x2 = 0, y2 = 0;     // For Line shapes
    };

    /**
     * @brief Creates a shape by name with given parameters
     * @param shapeName The type of shape to create (case-insensitive)
     * @param params The shape parameters
     * @return A unique pointer to the created shape, or nullptr if type unknown
     */
    static std::unique_ptr<Shape> create(const std::string& shapeName, const ShapeParams& params);

    /**
     * @brief Creates a shape with simple parameters (backward compatible)
     */
    static std::unique_ptr<Shape> createSimple(const std::string& shapeName, int x, int y);

    /**
     * @brief Creates a shape from parsed file line (extended format)
     * Format: ShapeName, x, y [, width, height, fillColor, borderColor, borderWidth, filled]
     */
    static std::unique_ptr<Shape> createFromLine(const std::vector<std::string>& tokens);

    /**
     * @brief Gets list of all available shape types
     */
    static std::vector<std::string> getAvailableShapes();

    /**
     * @brief Checks if a shape type is valid
     */
    static bool isValidShape(const std::string& shapeName);

    /**
     * @brief Gets a random color for variety
     */
    static Color getRandomColor();

    /**
     * @brief Gets the default color for a shape type
     */
    static Color getDefaultColor(const std::string& shapeName);

private:
    // Type alias for shape creator function
    using CreatorFunc = std::function<std::unique_ptr<Shape>(const ShapeParams&)>;

    // Gets the shape creator registry
    static const std::map<std::string, CreatorFunc>& getCreatorMap();
};

#endif // SHAPEFACTORY_H

