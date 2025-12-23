#include "Shape.h"
#include "Canvas.h"
#include <iostream>
#include <sstream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============== Base Shape Implementation ==============

Shape::Shape(const std::string& name, int x, int y, int width, int height,
             const Color& fillColor, const Color& borderColor,
             int borderWidth, bool filled)
    : name(name), x(x), y(y), width(width), height(height),
      fillColor(fillColor), borderColor(borderColor),
      borderWidth(borderWidth), rotation(0.0), filled(filled) {
}

void Shape::display() const {
    std::cout << "  " << name << " at (" << x << ", " << y << ") "
              << "[" << width << "x" << height << "] "
              << "Color: " << fillColor.getName() << std::endl;
}

void Shape::displayColored() const {
    std::cout << "  " << fillColor.toAnsiFg() << getAsciiIcon() << Color::resetAnsi()
              << " " << name << " at (" << x << ", " << y << ") "
              << "[" << width << "x" << height << "] "
              << fillColor.toAnsiFg() << "■" << Color::resetAnsi() 
              << " " << fillColor.getName() << std::endl;
}

std::string Shape::toJSON() const {
    std::ostringstream oss;
    oss << "{\n";
    oss << "      \"type\": \"" << name << "\",\n";
    oss << "      \"x\": " << x << ",\n";
    oss << "      \"y\": " << y << ",\n";
    oss << "      \"width\": " << width << ",\n";
    oss << "      \"height\": " << height << ",\n";
    oss << "      \"fillColor\": {\n";
    oss << "        \"name\": \"" << fillColor.getName() << "\",\n";
    oss << "        \"r\": " << (int)fillColor.r << ",\n";
    oss << "        \"g\": " << (int)fillColor.g << ",\n";
    oss << "        \"b\": " << (int)fillColor.b << "\n";
    oss << "      },\n";
    oss << "      \"borderColor\": {\n";
    oss << "        \"name\": \"" << borderColor.getName() << "\",\n";
    oss << "        \"r\": " << (int)borderColor.r << ",\n";
    oss << "        \"g\": " << (int)borderColor.g << ",\n";
    oss << "        \"b\": " << (int)borderColor.b << "\n";
    oss << "      },\n";
    oss << "      \"borderWidth\": " << borderWidth << ",\n";
    oss << "      \"rotation\": " << rotation << ",\n";
    oss << "      \"filled\": " << (filled ? "true" : "false") << "\n";
    oss << "    }";
    return oss.str();
}

// ============== Circle Implementation ==============

Circle::Circle(int x, int y, int radius, const Color& fillColor,
               const Color& borderColor, int borderWidth, bool filled)
    : Shape("Circle", x, y, radius * 2, radius * 2, fillColor, borderColor, borderWidth, filled) {
}

void Circle::render(Canvas& canvas) const {
    int radius = width / 2;
    int cx = x + radius;
    int cy = y + radius;
    
    if (filled) {
        canvas.fillCircle(cx, cy, radius, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawCircle(cx, cy, radius, borderColor, borderWidth);
    }
}

std::string Circle::toSVG() const {
    std::ostringstream oss;
    int r = width / 2;
    oss << "<circle cx=\"" << (x + r) << "\" cy=\"" << (y + r) << "\" r=\"" << r << "\" "
        << "fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Circle::getAsciiIcon() const {
    return "●";
}

std::unique_ptr<Shape> Circle::clone() const {
    return std::make_unique<Circle>(*this);
}

// ============== Rectangle Implementation ==============

Rectangle::Rectangle(int x, int y, int width, int height, const Color& fillColor,
                     const Color& borderColor, int borderWidth, bool filled)
    : Shape("Rectangle", x, y, width, height, fillColor, borderColor, borderWidth, filled) {
}

void Rectangle::render(Canvas& canvas) const {
    if (filled) {
        canvas.fillRect(x, y, width, height, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawRect(x, y, width, height, borderColor, borderWidth);
    }
}

std::string Rectangle::toSVG() const {
    std::ostringstream oss;
    oss << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\" "
        << "fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Rectangle::getAsciiIcon() const {
    return "▬";
}

std::unique_ptr<Shape> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);
}

// ============== Square Implementation ==============

Square::Square(int x, int y, int size, const Color& fillColor,
               const Color& borderColor, int borderWidth, bool filled)
    : Rectangle(x, y, size, size, fillColor, borderColor, borderWidth, filled) {
    name = "Square";
}

std::string Square::getAsciiIcon() const {
    return "■";
}

std::unique_ptr<Shape> Square::clone() const {
    return std::make_unique<Square>(*this);
}

// ============== Triangle Implementation ==============

Triangle::Triangle(int x, int y, int base, int height, const Color& fillColor,
                   const Color& borderColor, int borderWidth, bool filled)
    : Shape("Triangle", x, y, base, height, fillColor, borderColor, borderWidth, filled) {
}

void Triangle::render(Canvas& canvas) const {
    // Triangle points: top-center, bottom-left, bottom-right
    int x1 = x + width / 2;
    int y1 = y;
    int x2 = x;
    int y2 = y + height;
    int x3 = x + width;
    int y3 = y + height;
    
    if (filled) {
        canvas.fillTriangle(x1, y1, x2, y2, x3, y3, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawLine(x1, y1, x2, y2, borderColor, borderWidth);
        canvas.drawLine(x2, y2, x3, y3, borderColor, borderWidth);
        canvas.drawLine(x3, y3, x1, y1, borderColor, borderWidth);
    }
}

std::string Triangle::toSVG() const {
    std::ostringstream oss;
    int x1 = x + width / 2, y1 = y;
    int x2 = x, y2 = y + height;
    int x3 = x + width, y3 = y + height;
    
    oss << "<polygon points=\"" << x1 << "," << y1 << " " << x2 << "," << y2 << " " << x3 << "," << y3 << "\" "
        << "fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Triangle::getAsciiIcon() const {
    return "▲";
}

std::unique_ptr<Shape> Triangle::clone() const {
    return std::make_unique<Triangle>(*this);
}

// ============== Diamond Implementation ==============

Diamond::Diamond(int x, int y, int width, int height, const Color& fillColor,
                 const Color& borderColor, int borderWidth, bool filled)
    : Shape("Diamond", x, y, width, height, fillColor, borderColor, borderWidth, filled) {
}

void Diamond::render(Canvas& canvas) const {
    std::vector<std::pair<int, int>> points = {
        {x + width / 2, y},           // top
        {x + width, y + height / 2},  // right
        {x + width / 2, y + height},  // bottom
        {x, y + height / 2}           // left
    };
    
    if (filled) {
        canvas.fillPolygon(points, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawPolygon(points, borderColor, borderWidth);
    }
}

std::string Diamond::toSVG() const {
    std::ostringstream oss;
    oss << "<polygon points=\""
        << (x + width / 2) << "," << y << " "
        << (x + width) << "," << (y + height / 2) << " "
        << (x + width / 2) << "," << (y + height) << " "
        << x << "," << (y + height / 2) << "\" "
        << "fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Diamond::getAsciiIcon() const {
    return "◆";
}

std::unique_ptr<Shape> Diamond::clone() const {
    return std::make_unique<Diamond>(*this);
}

// ============== Star Implementation ==============

Star::Star(int x, int y, int size, const Color& fillColor,
           const Color& borderColor, int borderWidth, bool filled)
    : Shape("Star", x, y, size, size, fillColor, borderColor, borderWidth, filled) {
}

void Star::render(Canvas& canvas) const {
    int cx = x + width / 2;
    int cy = y + height / 2;
    int outerR = width / 2;
    int innerR = outerR * 2 / 5;
    
    std::vector<std::pair<int, int>> points;
    for (int i = 0; i < 10; ++i) {
        double angle = M_PI / 2 + i * M_PI / 5;
        int r = (i % 2 == 0) ? outerR : innerR;
        points.push_back({
            cx + static_cast<int>(r * std::cos(angle)),
            cy - static_cast<int>(r * std::sin(angle))
        });
    }
    
    if (filled) {
        canvas.fillPolygon(points, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawPolygon(points, borderColor, borderWidth);
    }
}

std::string Star::toSVG() const {
    std::ostringstream oss;
    int cx = x + width / 2;
    int cy = y + height / 2;
    int outerR = width / 2;
    int innerR = outerR * 2 / 5;
    
    oss << "<polygon points=\"";
    for (int i = 0; i < 10; ++i) {
        double angle = M_PI / 2 + i * M_PI / 5;
        int r = (i % 2 == 0) ? outerR : innerR;
        if (i > 0) oss << " ";
        oss << (cx + static_cast<int>(r * std::cos(angle))) << ","
            << (cy - static_cast<int>(r * std::sin(angle)));
    }
    oss << "\" fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Star::getAsciiIcon() const {
    return "★";
}

std::unique_ptr<Shape> Star::clone() const {
    return std::make_unique<Star>(*this);
}

// ============== Hexagon Implementation ==============

Hexagon::Hexagon(int x, int y, int size, const Color& fillColor,
                 const Color& borderColor, int borderWidth, bool filled)
    : Shape("Hexagon", x, y, size, size, fillColor, borderColor, borderWidth, filled) {
}

void Hexagon::render(Canvas& canvas) const {
    int cx = x + width / 2;
    int cy = y + height / 2;
    int r = width / 2;
    
    std::vector<std::pair<int, int>> points;
    for (int i = 0; i < 6; ++i) {
        double angle = i * M_PI / 3;
        points.push_back({
            cx + static_cast<int>(r * std::cos(angle)),
            cy + static_cast<int>(r * std::sin(angle))
        });
    }
    
    if (filled) {
        canvas.fillPolygon(points, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawPolygon(points, borderColor, borderWidth);
    }
}

std::string Hexagon::toSVG() const {
    std::ostringstream oss;
    int cx = x + width / 2;
    int cy = y + height / 2;
    int r = width / 2;
    
    oss << "<polygon points=\"";
    for (int i = 0; i < 6; ++i) {
        double angle = i * M_PI / 3;
        if (i > 0) oss << " ";
        oss << (cx + static_cast<int>(r * std::cos(angle))) << ","
            << (cy + static_cast<int>(r * std::sin(angle)));
    }
    oss << "\" fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Hexagon::getAsciiIcon() const {
    return "⬡";
}

std::unique_ptr<Shape> Hexagon::clone() const {
    return std::make_unique<Hexagon>(*this);
}

// ============== Pentagon Implementation ==============

Pentagon::Pentagon(int x, int y, int size, const Color& fillColor,
                   const Color& borderColor, int borderWidth, bool filled)
    : Shape("Pentagon", x, y, size, size, fillColor, borderColor, borderWidth, filled) {
}

void Pentagon::render(Canvas& canvas) const {
    int cx = x + width / 2;
    int cy = y + height / 2;
    int r = width / 2;
    
    std::vector<std::pair<int, int>> points;
    for (int i = 0; i < 5; ++i) {
        double angle = M_PI / 2 + i * 2 * M_PI / 5;
        points.push_back({
            cx + static_cast<int>(r * std::cos(angle)),
            cy - static_cast<int>(r * std::sin(angle))
        });
    }
    
    if (filled) {
        canvas.fillPolygon(points, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawPolygon(points, borderColor, borderWidth);
    }
}

std::string Pentagon::toSVG() const {
    std::ostringstream oss;
    int cx = x + width / 2;
    int cy = y + height / 2;
    int r = width / 2;
    
    oss << "<polygon points=\"";
    for (int i = 0; i < 5; ++i) {
        double angle = M_PI / 2 + i * 2 * M_PI / 5;
        if (i > 0) oss << " ";
        oss << (cx + static_cast<int>(r * std::cos(angle))) << ","
            << (cy - static_cast<int>(r * std::sin(angle)));
    }
    oss << "\" fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Pentagon::getAsciiIcon() const {
    return "⬠";
}

std::unique_ptr<Shape> Pentagon::clone() const {
    return std::make_unique<Pentagon>(*this);
}

// ============== Oval Implementation ==============

Oval::Oval(int x, int y, int width, int height, const Color& fillColor,
           const Color& borderColor, int borderWidth, bool filled)
    : Shape("Oval", x, y, width, height, fillColor, borderColor, borderWidth, filled) {
}

void Oval::render(Canvas& canvas) const {
    int cx = x + width / 2;
    int cy = y + height / 2;
    int rx = width / 2;
    int ry = height / 2;
    
    if (filled) {
        canvas.fillEllipse(cx, cy, rx, ry, fillColor);
    }
    if (borderWidth > 0) {
        // Draw ellipse border by drawing outer - inner
        for (int py = -ry; py <= ry; ++py) {
            for (int px = -rx; px <= rx; ++px) {
                double outer = (double)(px * px) / (rx * rx) + (double)(py * py) / (ry * ry);
                double inner = (double)(px * px) / ((rx - borderWidth) * (rx - borderWidth)) +
                               (double)(py * py) / ((ry - borderWidth) * (ry - borderWidth));
                if (outer <= 1.0 && inner > 1.0) {
                    canvas.setPixel(cx + px, cy + py, borderColor);
                }
            }
        }
    }
}

std::string Oval::toSVG() const {
    std::ostringstream oss;
    int rx = width / 2;
    int ry = height / 2;
    oss << "<ellipse cx=\"" << (x + rx) << "\" cy=\"" << (y + ry) << "\" rx=\"" << rx << "\" ry=\"" << ry << "\" "
        << "fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Oval::getAsciiIcon() const {
    return "⬭";
}

std::unique_ptr<Shape> Oval::clone() const {
    return std::make_unique<Oval>(*this);
}

// ============== Trapezoid Implementation ==============

Trapezoid::Trapezoid(int x, int y, int width, int height, const Color& fillColor,
                     const Color& borderColor, int borderWidth, bool filled)
    : Shape("Trapezoid", x, y, width, height, fillColor, borderColor, borderWidth, filled) {
}

void Trapezoid::render(Canvas& canvas) const {
    int inset = width / 4;
    std::vector<std::pair<int, int>> points = {
        {x + inset, y},              // top-left
        {x + width - inset, y},      // top-right
        {x + width, y + height},     // bottom-right
        {x, y + height}              // bottom-left
    };
    
    if (filled) {
        canvas.fillPolygon(points, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawPolygon(points, borderColor, borderWidth);
    }
}

std::string Trapezoid::toSVG() const {
    std::ostringstream oss;
    int inset = width / 4;
    oss << "<polygon points=\""
        << (x + inset) << "," << y << " "
        << (x + width - inset) << "," << y << " "
        << (x + width) << "," << (y + height) << " "
        << x << "," << (y + height) << "\" "
        << "fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Trapezoid::getAsciiIcon() const {
    return "⏢";
}

std::unique_ptr<Shape> Trapezoid::clone() const {
    return std::make_unique<Trapezoid>(*this);
}

// ============== Rhombus Implementation ==============

Rhombus::Rhombus(int x, int y, int width, int height, const Color& fillColor,
                 const Color& borderColor, int borderWidth, bool filled)
    : Shape("Rhombus", x, y, width, height, fillColor, borderColor, borderWidth, filled) {
}

void Rhombus::render(Canvas& canvas) const {
    std::vector<std::pair<int, int>> points = {
        {x + width / 2, y},           // top
        {x + width, y + height / 2},  // right
        {x + width / 2, y + height},  // bottom
        {x, y + height / 2}           // left
    };
    
    if (filled) {
        canvas.fillPolygon(points, fillColor);
    }
    if (borderWidth > 0) {
        canvas.drawPolygon(points, borderColor, borderWidth);
    }
}

std::string Rhombus::toSVG() const {
    std::ostringstream oss;
    oss << "<polygon points=\""
        << (x + width / 2) << "," << y << " "
        << (x + width) << "," << (y + height / 2) << " "
        << (x + width / 2) << "," << (y + height) << " "
        << x << "," << (y + height / 2) << "\" "
        << "fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke=\"rgb(" << (int)borderColor.r << "," << (int)borderColor.g << "," << (int)borderColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Rhombus::getAsciiIcon() const {
    return "◇";
}

std::unique_ptr<Shape> Rhombus::clone() const {
    return std::make_unique<Rhombus>(*this);
}

// ============== Line Implementation ==============

Line::Line(int x1, int y1, int x2, int y2, const Color& color, int lineWidth)
    : Shape("Line", x1, y1, std::abs(x2 - x1), std::abs(y2 - y1), color, color, lineWidth, false),
      x2(x2), y2(y2) {
}

void Line::render(Canvas& canvas) const {
    canvas.drawLine(x, y, x2, y2, fillColor, borderWidth);
}

std::string Line::toSVG() const {
    std::ostringstream oss;
    oss << "<line x1=\"" << x << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" "
        << "stroke=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\" "
        << "stroke-width=\"" << borderWidth << "\"/>";
    return oss.str();
}

std::string Line::getAsciiIcon() const {
    return "─";
}

std::unique_ptr<Shape> Line::clone() const {
    return std::make_unique<Line>(*this);
}

// ============== Text Implementation ==============

Text::Text(int x, int y, const std::string& text, const Color& color, int fontSize)
    : Shape("Text", x, y, static_cast<int>(text.length()) * 8, fontSize, color, color, 0, true),
      content(text), fontSize(fontSize) {
}

void Text::render(Canvas& canvas) const {
    // Render text as colored rectangles (simplified - actual font rendering would be complex)
    int charWidth = 8;
    int charHeight = fontSize;
    for (size_t i = 0; i < content.size(); ++i) {
        if (content[i] != ' ') {
            canvas.fillRect(x + static_cast<int>(i) * charWidth, y, charWidth - 1, charHeight, fillColor);
        }
    }
}

std::string Text::toSVG() const {
    std::ostringstream oss;
    oss << "<text x=\"" << x << "\" y=\"" << (y + fontSize) << "\" "
        << "font-size=\"" << fontSize << "\" "
        << "fill=\"rgb(" << (int)fillColor.r << "," << (int)fillColor.g << "," << (int)fillColor.b << ")\">"
        << content << "</text>";
    return oss.str();
}

std::string Text::getAsciiIcon() const {
    return "T";
}

std::unique_ptr<Shape> Text::clone() const {
    return std::make_unique<Text>(*this);
}
