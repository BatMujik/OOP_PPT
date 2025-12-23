#include "Slide.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

Slide::Slide(int width, int height) 
    : backgroundColor(30, 30, 45), canvasWidth(width), canvasHeight(height) {
}

Slide::Slide(const Slide& other)
    : backgroundColor(other.backgroundColor), 
      title(other.title),
      canvasWidth(other.canvasWidth),
      canvasHeight(other.canvasHeight) {
    // Deep copy shapes
    for (const auto& shape : other.shapes) {
        shapes.push_back(std::shared_ptr<Shape>(shape->clone().release()));
    }
}

Slide& Slide::operator=(const Slide& other) {
    if (this != &other) {
        shapes.clear();
        backgroundColor = other.backgroundColor;
        title = other.title;
        canvasWidth = other.canvasWidth;
        canvasHeight = other.canvasHeight;
        
        for (const auto& shape : other.shapes) {
            shapes.push_back(std::shared_ptr<Shape>(shape->clone().release()));
        }
    }
    return *this;
}

void Slide::addShape(std::shared_ptr<Shape> shape) {
    shapes.push_back(shape);
}

bool Slide::removeShape(size_t index) {
    if (index < shapes.size()) {
        shapes.erase(shapes.begin() + index);
        return true;
    }
    return false;
}

std::shared_ptr<Shape> Slide::getShape(size_t index) const {
    if (index < shapes.size()) {
        return shapes[index];
    }
    return nullptr;
}

void Slide::show() const {
    if (shapes.empty()) {
        std::cout << "  [Empty slide]" << std::endl;
    } else {
        for (const auto& shape : shapes) {
            shape->display();
        }
    }
}

void Slide::showColored() const {
    if (shapes.empty()) {
        std::cout << Color::Gray().toAnsiFg() << "  [Empty slide]" 
                  << Color::resetAnsi() << std::endl;
    } else {
        for (const auto& shape : shapes) {
            shape->displayColored();
        }
    }
}

Canvas Slide::render() const {
    Canvas canvas(canvasWidth, canvasHeight, backgroundColor);
    
    for (const auto& shape : shapes) {
        shape->render(canvas);
    }
    
    return canvas;
}

void Slide::displayVisual() const {
    Canvas canvas = render();
    canvas.displayAscii();
}

bool Slide::exportBMP(const std::string& filename) const {
    Canvas canvas = render();
    return canvas.exportBMP(filename);
}

bool Slide::exportSVG(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << toSVG();
    file.close();
    return true;
}

bool Slide::exportJSON(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << toJSON();
    file.close();
    return true;
}

std::string Slide::toSVG() const {
    std::ostringstream oss;
    
    oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    oss << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << canvasWidth 
        << "\" height=\"" << canvasHeight << "\">\n";
    
    // Background
    oss << "  <rect width=\"100%\" height=\"100%\" fill=\"rgb("
        << (int)backgroundColor.r << "," << (int)backgroundColor.g << "," 
        << (int)backgroundColor.b << ")\"/>\n";
    
    // Title if present
    if (!title.empty()) {
        oss << "  <text x=\"10\" y=\"25\" font-size=\"18\" fill=\"white\" font-weight=\"bold\">"
            << title << "</text>\n";
    }
    
    // Shapes
    for (const auto& shape : shapes) {
        oss << "  " << shape->toSVG() << "\n";
    }
    
    oss << "</svg>";
    
    return oss.str();
}

std::string Slide::toJSON() const {
    std::ostringstream oss;
    
    oss << "{\n";
    oss << "  \"slide\": {\n";
    oss << "    \"title\": \"" << title << "\",\n";
    oss << "    \"canvasWidth\": " << canvasWidth << ",\n";
    oss << "    \"canvasHeight\": " << canvasHeight << ",\n";
    oss << "    \"backgroundColor\": {\n";
    oss << "      \"r\": " << (int)backgroundColor.r << ",\n";
    oss << "      \"g\": " << (int)backgroundColor.g << ",\n";
    oss << "      \"b\": " << (int)backgroundColor.b << "\n";
    oss << "    },\n";
    oss << "    \"shapes\": [\n";
    
    for (size_t i = 0; i < shapes.size(); ++i) {
        oss << "    " << shapes[i]->toJSON();
        if (i < shapes.size() - 1) {
            oss << ",";
        }
        oss << "\n";
    }
    
    oss << "    ]\n";
    oss << "  }\n";
    oss << "}";
    
    return oss.str();
}

bool Slide::isEmpty() const {
    return shapes.empty();
}

size_t Slide::getShapeCount() const {
    return shapes.size();
}

void Slide::listShapes() const {
    if (shapes.empty()) {
        std::cout << Color::Gray().toAnsiFg() << "  No shapes on this slide." 
                  << Color::resetAnsi() << std::endl;
        return;
    }
    
    std::cout << Color::Cyan().toAnsiFg() << "\n  === Shape List ===" 
              << Color::resetAnsi() << std::endl;
    
    for (size_t i = 0; i < shapes.size(); ++i) {
        const auto& shape = shapes[i];
        std::cout << "  " << Color::Yellow().toAnsiFg() << std::setw(2) << (i + 1) << ". "
                  << Color::resetAnsi()
                  << shape->getFillColor().toAnsiFg() << shape->getAsciiIcon() << " "
                  << Color::resetAnsi()
                  << std::setw(12) << std::left << shape->getName()
                  << " at (" << std::setw(3) << shape->getX() << ", " << std::setw(3) << shape->getY() << ")"
                  << " [" << shape->getWidth() << "x" << shape->getHeight() << "]"
                  << " " << shape->getFillColor().toAnsiFg() << "■ " << shape->getFillColor().getName()
                  << Color::resetAnsi() << std::endl;
    }
    
    std::cout << Color::Cyan().toAnsiFg() << "  ==================" 
              << Color::resetAnsi() << "\n" << std::endl;
}

std::string Slide::toFileFormat() const {
    std::ostringstream oss;
    
    for (const auto& shape : shapes) {
        oss << shape->getName() << ", " 
            << shape->getX() << ", " 
            << shape->getY() << ", "
            << shape->getWidth() << ", "
            << shape->getHeight() << ", "
            << shape->getFillColor().getName() << ", "
            << shape->getBorderColor().getName() << ", "
            << shape->getBorderWidth() << ", "
            << (shape->isFilled() ? "true" : "false") << "\n";
    }
    
    return oss.str();
}
