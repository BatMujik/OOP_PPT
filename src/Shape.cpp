#include "Shape.h"
#include <iostream>

Shape::Shape(const std::string& name, int x, int y) 
    : name(name), x(x), y(y) {
}

std::string Shape::getName() const {
    return name;
}

int Shape::getX() const {
    return x;
}

int Shape::getY() const {
    return y;
}

void Shape::display() const {
    std::cout << "  " << name << " at position (" << x << ", " << y << ")" << std::endl;
}
