#include "Slide.h"
#include <iostream>

Slide::Slide() {
}

void Slide::addShape(std::shared_ptr<Shape> shape) {
    shapes.push_back(shape);
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

bool Slide::isEmpty() const {
    return shapes.empty();
}

size_t Slide::getShapeCount() const {
    return shapes.size();
}
