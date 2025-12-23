#ifndef SLIDE_H
#define SLIDE_H

#include "Shape.h"
#include "Canvas.h"
#include "Color.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Represents a slide containing multiple shapes with visual rendering
 */
class Slide {
private:
    std::vector<std::shared_ptr<Shape>> shapes;
    Color backgroundColor;
    std::string title;
    int canvasWidth;
    int canvasHeight;

public:
    /**
     * @brief Default constructor with standard canvas size
     */
    Slide(int width = 320, int height = 180);

    /**
     * @brief Copy constructor
     */
    Slide(const Slide& other);

    /**
     * @brief Assignment operator
     */
    Slide& operator=(const Slide& other);

    /**
     * @brief Adds a shape to the slide
     * @param shape Shared pointer to the shape to add
     */
    void addShape(std::shared_ptr<Shape> shape);

    /**
     * @brief Removes a shape by index (0-based)
     * @return True if removed successfully
     */
    bool removeShape(size_t index);

    /**
     * @brief Gets a shape by index
     */
    std::shared_ptr<Shape> getShape(size_t index) const;

    /**
     * @brief Displays all shapes in text format
     */
    void show() const;

    /**
     * @brief Displays all shapes with colored formatting
     */
    void showColored() const;

    /**
     * @brief Renders the slide to a canvas
     */
    Canvas render() const;

    /**
     * @brief Displays the slide as ASCII art in terminal
     */
    void displayVisual() const;

    /**
     * @brief Exports the slide to a BMP image file
     * @return True if successful
     */
    bool exportBMP(const std::string& filename) const;

    /**
     * @brief Exports the slide to an SVG file
     * @return True if successful
     */
    bool exportSVG(const std::string& filename) const;

    /**
     * @brief Exports the slide to a JSON file
     * @return True if successful
     */
    bool exportJSON(const std::string& filename) const;

    /**
     * @brief Gets SVG content as a string
     */
    std::string toSVG() const;

    /**
     * @brief Gets JSON content as a string
     */
    std::string toJSON() const;

    /**
     * @brief Checks if the slide is empty
     */
    bool isEmpty() const;

    /**
     * @brief Gets the number of shapes on the slide
     */
    size_t getShapeCount() const;

    /**
     * @brief Lists all shapes with their properties
     */
    void listShapes() const;

    // Property accessors
    void setBackgroundColor(const Color& color) { backgroundColor = color; }
    Color getBackgroundColor() const { return backgroundColor; }
    void setTitle(const std::string& t) { title = t; }
    std::string getTitle() const { return title; }
    int getCanvasWidth() const { return canvasWidth; }
    int getCanvasHeight() const { return canvasHeight; }

    /**
     * @brief Converts slide to file format string for saving
     */
    std::string toFileFormat() const;
};

#endif // SLIDE_H
