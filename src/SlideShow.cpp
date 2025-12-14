#include "SlideShow.h"
#include <iostream>

SlideShow::SlideShow(const std::string& filename) 
    : currentSlideIndex(0), filename(filename) {
}

void SlideShow::addSlide(const Slide& slide) {
    slides.push_back(slide);
}

bool SlideShow::next() {
    if (currentSlideIndex < static_cast<int>(slides.size()) - 1) {
        currentSlideIndex++;
        return true;
    }
    return false;
}

bool SlideShow::prev() {
    if (currentSlideIndex > 0) {
        currentSlideIndex--;
        return true;
    }
    return false;
}

void SlideShow::show() const {
    if (slides.empty()) {
        std::cout << "[WARN] No slides to display." << std::endl;
        return;
    }
    std::cout << "\n--- Slide " << (currentSlideIndex + 1) << "/" << slides.size() << " ---" << std::endl;
    slides[currentSlideIndex].show();
    std::cout << std::endl;
}

bool SlideShow::gotoSlide(int slideNumber) {
    if (slideNumber < 1 || slideNumber > static_cast<int>(slides.size())) {
        return false;
    }
    currentSlideIndex = slideNumber - 1;
    return true;
}

int SlideShow::getCurrentSlideNumber() const {
    return currentSlideIndex + 1;
}

int SlideShow::getTotalSlides() const {
    return static_cast<int>(slides.size());
}

std::string SlideShow::getFilename() const {
    return filename;
}

bool SlideShow::isEmpty() const {
    return slides.empty();
}

Slide* SlideShow::getCurrentSlide() {
    if (slides.empty()) {
        return nullptr;
    }
    return &slides[currentSlideIndex];
}
