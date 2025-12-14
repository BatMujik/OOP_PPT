#ifndef SLIDE_H
#define SLIDE_H

#include "Shape.h"
#include <vector>
#include <memory>

/**
 * @brief Represents a slide containing multiple shapes
 */
class Slide {
private:
    std::vector<std::shared_ptr<Shape>> shapes;

public:
    /**
     * @brief Default constructor
     */
    Slide();

    /**
     * @brief Adds a shape to the slide
     * @param shape Shared pointer to the shape to add
     */
    void addShape(std::shared_ptr<Shape> shape);

    /**
     * @brief Displays all shapes on the slide
     */
    void show() const;

    /**
     * @brief Checks if the slide is empty
     * @return True if the slide has no shapes
     */
    bool isEmpty() const;

    /**
     * @brief Gets the number of shapes on the slide
     * @return The count of shapes
     */
    size_t getShapeCount() const;
};

#endif // SLIDE_H
