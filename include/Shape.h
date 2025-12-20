#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <memory>
#include <vector>
#include "Color.h"

// Forward declaration
class Canvas;

/**
 * @brief Abstract base class for all shapes with visual properties
 * Demonstrates polymorphism and inheritance in OOP
 */
class Shape {
protected:
    std::string name;
    int x;
    int y;
    int width;
    int height;
    Color fillColor;
    Color borderColor;
    int borderWidth;
    double rotation;  // in degrees
    bool filled;

public:
    /**
     * @brief Constructs a Shape with all properties
     */
    Shape(const std::string& name, int x, int y, int width = 40, int height = 20,
          const Color& fillColor = Color::Blue(), const Color& borderColor = Color::White(),
          int borderWidth = 2, bool filled = true);

    /**
     * @brief Virtual destructor for proper polymorphic deletion
     */
    virtual ~Shape() = default;

    // Getters
    std::string getName() const { return name; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Color getFillColor() const { return fillColor; }
    Color getBorderColor() const { return borderColor; }
    int getBorderWidth() const { return borderWidth; }
    double getRotation() const { return rotation; }
    bool isFilled() const { return filled; }

    // Setters
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void setSize(int w, int h) { width = w; height = h; }
    void setFillColor(const Color& color) { fillColor = color; }
    void setBorderColor(const Color& color) { borderColor = color; }
    void setBorderWidth(int w) { borderWidth = w; }
    void setRotation(double r) { rotation = r; }
    void setFilled(bool f) { filled = f; }

    /**
     * @brief Displays shape info in text format
     */
    virtual void display() const;

    /**
     * @brief Displays shape with colorful formatting
     */
    virtual void displayColored() const;

    /**
     * @brief Renders shape to a canvas for ASCII/image output
     */
    virtual void render(Canvas& canvas) const = 0;

    /**
     * @brief Gets SVG representation of the shape
     */
    virtual std::string toSVG() const = 0;

    /**
     * @brief Gets ASCII art representation (small icon)
     */
    virtual std::string getAsciiIcon() const = 0;

    /**
     * @brief Clone the shape (for copying)
     */
    virtual std::unique_ptr<Shape> clone() const = 0;
};

// ============== Concrete Shape Classes ==============

class Circle : public Shape {
public:
    Circle(int x, int y, int radius = 20,
           const Color& fillColor = Color::Red(),
           const Color& borderColor = Color::White(),
           int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;

    int getRadius() const { return width / 2; }
};

class Rectangle : public Shape {
public:
    Rectangle(int x, int y, int width = 50, int height = 30,
              const Color& fillColor = Color::Blue(),
              const Color& borderColor = Color::White(),
              int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Square : public Rectangle {
public:
    Square(int x, int y, int size = 30,
           const Color& fillColor = Color::Green(),
           const Color& borderColor = Color::White(),
           int borderWidth = 2, bool filled = true);

    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Triangle : public Shape {
public:
    Triangle(int x, int y, int base = 40, int height = 30,
             const Color& fillColor = Color::Yellow(),
             const Color& borderColor = Color::White(),
             int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Diamond : public Shape {
public:
    Diamond(int x, int y, int width = 30, int height = 40,
            const Color& fillColor = Color::Cyan(),
            const Color& borderColor = Color::White(),
            int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Star : public Shape {
public:
    Star(int x, int y, int size = 40,
         const Color& fillColor = Color::Gold(),
         const Color& borderColor = Color::Orange(),
         int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Hexagon : public Shape {
public:
    Hexagon(int x, int y, int size = 35,
            const Color& fillColor = Color::Purple(),
            const Color& borderColor = Color::White(),
            int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Pentagon : public Shape {
public:
    Pentagon(int x, int y, int size = 35,
             const Color& fillColor = Color::Teal(),
             const Color& borderColor = Color::White(),
             int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Oval : public Shape {
public:
    Oval(int x, int y, int width = 50, int height = 30,
         const Color& fillColor = Color::Pink(),
         const Color& borderColor = Color::White(),
         int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Trapezoid : public Shape {
public:
    Trapezoid(int x, int y, int width = 50, int height = 30,
              const Color& fillColor = Color::Coral(),
              const Color& borderColor = Color::White(),
              int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Rhombus : public Shape {
public:
    Rhombus(int x, int y, int width = 40, int height = 50,
            const Color& fillColor = Color::Violet(),
            const Color& borderColor = Color::White(),
            int borderWidth = 2, bool filled = true);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;
};

class Line : public Shape {
private:
    int x2, y2;  // End point
public:
    Line(int x1, int y1, int x2, int y2,
         const Color& color = Color::White(),
         int lineWidth = 2);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;

    int getX2() const { return x2; }
    int getY2() const { return y2; }
};

class Text : public Shape {
private:
    std::string content;
    int fontSize;
public:
    Text(int x, int y, const std::string& text,
         const Color& color = Color::White(),
         int fontSize = 16);

    void render(Canvas& canvas) const override;
    std::string toSVG() const override;
    std::string getAsciiIcon() const override;
    std::unique_ptr<Shape> clone() const override;

    std::string getContent() const { return content; }
    void setContent(const std::string& text) { content = text; }
};

#endif // SHAPE_H
