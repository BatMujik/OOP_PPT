#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <string>
#include <fstream>
#include "Color.h"

/**
 * @brief A pixel canvas for rendering shapes and exporting images
 * Supports ASCII art display and BMP/SVG export
 */
class Canvas {
private:
    int width;
    int height;
    std::vector<std::vector<Color>> pixels;
    std::vector<std::vector<char>> asciiBuffer;
    std::vector<std::vector<Color>> asciiFgColors;
    Color backgroundColor;

public:
    /**
     * @brief Creates a canvas with specified dimensions
     */
    Canvas(int width, int height, const Color& bgColor = Color(30, 30, 40));

    /**
     * @brief Clears the canvas with background color
     */
    void clear();

    /**
     * @brief Sets a pixel at (x, y) with given color
     */
    void setPixel(int x, int y, const Color& color);

    /**
     * @brief Gets pixel color at (x, y)
     */
    Color getPixel(int x, int y) const;

    /**
     * @brief Sets an ASCII character at scaled position
     */
    void setAscii(int x, int y, char ch, const Color& color);

    /**
     * @brief Draws a filled rectangle
     */
    void fillRect(int x, int y, int w, int h, const Color& color);

    /**
     * @brief Draws a rectangle border
     */
    void drawRect(int x, int y, int w, int h, const Color& color, int thickness = 1);

    /**
     * @brief Draws a filled circle
     */
    void fillCircle(int cx, int cy, int radius, const Color& color);

    /**
     * @brief Draws a circle outline
     */
    void drawCircle(int cx, int cy, int radius, const Color& color, int thickness = 1);

    /**
     * @brief Draws a filled ellipse
     */
    void fillEllipse(int cx, int cy, int rx, int ry, const Color& color);

    /**
     * @brief Draws a line between two points
     */
    void drawLine(int x1, int y1, int x2, int y2, const Color& color, int thickness = 1);

    /**
     * @brief Draws a filled triangle
     */
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);

    /**
     * @brief Draws a filled polygon
     */
    void fillPolygon(const std::vector<std::pair<int, int>>& points, const Color& color);

    /**
     * @brief Draws polygon outline
     */
    void drawPolygon(const std::vector<std::pair<int, int>>& points, const Color& color, int thickness = 1);

    // ========== Export Functions ==========

    /**
     * @brief Exports the canvas to a BMP file (viewable on Windows)
     */
    bool exportBMP(const std::string& filename) const;

    /**
     * @brief Returns the canvas as ASCII art string with ANSI colors
     */
    std::string toAsciiArt() const;

    /**
     * @brief Displays the canvas as colored ASCII art in terminal
     */
    void displayAscii() const;

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    // Helper for point-in-polygon test
    bool pointInPolygon(int x, int y, const std::vector<std::pair<int, int>>& points) const;
    
    // Helper for triangle fill
    void fillFlatBottomTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
    void fillFlatTopTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
};

#endif // CANVAS_H

