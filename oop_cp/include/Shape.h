#ifndef SHAPE_H
#define SHAPE_H

#include <string>

/**
 * @brief Represents a shape with a name and position (x, y)
 */
class Shape {
private:
    std::string name;
    int x;
    int y;

public:
    /**
     * @brief Constructs a Shape with given parameters
     * @param name The name/type of the shape (e.g., Circle, Square)
     * @param x The x-coordinate position
     * @param y The y-coordinate position
     */
    Shape(const std::string& name, int x, int y);

    /**
     * @brief Gets the name of the shape
     * @return The shape's name
     */
    std::string getName() const;

    /**
     * @brief Gets the x-coordinate
     * @return The x position
     */
    int getX() const;

    /**
     * @brief Gets the y-coordinate
     * @return The y position
     */
    int getY() const;

    /**
     * @brief Displays the shape information
     */
    void display() const;
};

#endif // SHAPE_H
