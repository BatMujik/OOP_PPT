#include "SlideShow.h"
#include "Color.h"
#include <iostream>
#include <sstream>
#include <iomanip>

SlideShow::SlideShow(const std::string& filename) 
    : currentSlideIndex(0), filename(filename), modified(false) {
}

void SlideShow::addSlide(const Slide& slide) {
    slides.push_back(slide);
}

int SlideShow::addNewSlide() {
    slides.push_back(Slide());
    modified = true;
    return static_cast<int>(slides.size());
}

bool SlideShow::removeSlide(int slideNumber) {
    if (slideNumber < 1 || slideNumber > static_cast<int>(slides.size())) {
        return false;
    }
    
    slides.erase(slides.begin() + slideNumber - 1);
    modified = true;
    
    // Adjust current index if needed
    if (currentSlideIndex >= static_cast<int>(slides.size())) {
        currentSlideIndex = std::max(0, static_cast<int>(slides.size()) - 1);
    }
    
    return true;
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
        std::cout << Color::Yellow().toAnsiFg() << "[WARN] No slides to display." 
                  << Color::resetAnsi() << std::endl;
        return;
    }
    
    std::cout << Color::Cyan().toAnsiFg() 
              << "\n╔═══════════════════════════════════════════════════════════════╗"
              << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() << "║" << Color::resetAnsi()
              << "  " << Color::White().toAnsiFg() << "Slide " << (currentSlideIndex + 1) 
              << "/" << slides.size() << Color::resetAnsi();
    
    // Add title if present
    if (!slides[currentSlideIndex].getTitle().empty()) {
        std::cout << " - " << Color::Yellow().toAnsiFg() 
                  << slides[currentSlideIndex].getTitle() << Color::resetAnsi();
    }
    
    // Padding to align the right border
    std::cout << std::string(40, ' ') << Color::Cyan().toAnsiFg() << "║" 
              << Color::resetAnsi() << std::endl;
    std::cout << Color::Cyan().toAnsiFg() 
              << "╚═══════════════════════════════════════════════════════════════╝"
              << Color::resetAnsi() << "\n" << std::endl;
    
    slides[currentSlideIndex].showColored();
    std::cout << std::endl;
}

void SlideShow::showColored() const {
    show();  // Already colored
}

void SlideShow::showVisual() const {
    if (slides.empty()) {
        std::cout << Color::Yellow().toAnsiFg() << "[WARN] No slides to display." 
                  << Color::resetAnsi() << std::endl;
        return;
    }
    
    std::cout << Color::Cyan().toAnsiFg() 
              << "\n══════════════ Slide " << (currentSlideIndex + 1) << "/" << slides.size() 
              << " ══════════════" << Color::resetAnsi() << std::endl;
    
    if (!slides[currentSlideIndex].getTitle().empty()) {
        std::cout << Color::Yellow().toAnsiFg() << "  " 
                  << slides[currentSlideIndex].getTitle() 
                  << Color::resetAnsi() << std::endl;
    }
    
    slides[currentSlideIndex].displayVisual();
    
    // Also show shape list
    slides[currentSlideIndex].showColored();
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

const Slide* SlideShow::getCurrentSlide() const {
    if (slides.empty()) {
        return nullptr;
    }
    return &slides[currentSlideIndex];
}

bool SlideShow::exportCurrentSlideBMP(const std::string& filename) const {
    if (slides.empty()) {
        return false;
    }
    return slides[currentSlideIndex].exportBMP(filename);
}

bool SlideShow::exportCurrentSlideSVG(const std::string& filename) const {
    if (slides.empty()) {
        return false;
    }
    return slides[currentSlideIndex].exportSVG(filename);
}

void SlideShow::exportAllSlidesBMP(const std::string& prefix) const {
    for (size_t i = 0; i < slides.size(); ++i) {
        std::ostringstream oss;
        oss << prefix << "_" << (i + 1) << ".bmp";
        if (slides[i].exportBMP(oss.str())) {
            std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                      << "Exported: " << oss.str() << std::endl;
        } else {
            std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                      << "Failed to export: " << oss.str() << std::endl;
        }
    }
}

void SlideShow::exportAllSlidesSVG(const std::string& prefix) const {
    for (size_t i = 0; i < slides.size(); ++i) {
        std::ostringstream oss;
        oss << prefix << "_" << (i + 1) << ".svg";
        if (slides[i].exportSVG(oss.str())) {
            std::cout << Color::Green().toAnsiFg() << "[OK] " << Color::resetAnsi()
                      << "Exported: " << oss.str() << std::endl;
        } else {
            std::cout << Color::Red().toAnsiFg() << "[ERR] " << Color::resetAnsi()
                      << "Failed to export: " << oss.str() << std::endl;
        }
    }
}

bool SlideShow::save(const std::string& newFilename) {
    std::string saveFilename = newFilename.empty() ? filename : newFilename;
    
    std::ofstream file(saveFilename);
    if (!file.is_open()) {
        return false;
    }
    
    for (size_t i = 0; i < slides.size(); ++i) {
        file << slides[i].toFileFormat();
        
        // Add slide separator between slides (not after last)
        if (i < slides.size() - 1) {
            file << "---\n";
        }
    }
    
    file.close();
    
    if (!newFilename.empty()) {
        filename = newFilename;
    }
    modified = false;
    
    return true;
}
