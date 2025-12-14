#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include "Slide.h"
#include <vector>
#include <string>

/**
 * @brief Manages a collection of slides for a single presentation file
 */
class SlideShow {
private:
    std::vector<Slide> slides;
    int currentSlideIndex;
    std::string filename;

public:
    /**
     * @brief Constructs a SlideShow with a filename
     * @param filename The name of the presentation file
     */
    SlideShow(const std::string& filename);

    /**
     * @brief Adds a slide to the slideshow
     * @param slide The slide to add
     */
    void addSlide(const Slide& slide);

    /**
     * @brief Moves to the next slide
     * @return True if successful, false if already at the last slide
     */
    bool next();

    /**
     * @brief Moves to the previous slide
     * @return True if successful, false if already at the first slide
     */
    bool prev();

    /**
     * @brief Displays the current slide
     */
    void show() const;

    /**
     * @brief Jumps to a specific slide (1-based index)
     * @param slideNumber The slide number to jump to
     * @return True if successful, false if slide number is invalid
     */
    bool gotoSlide(int slideNumber);

    /**
     * @brief Gets the current slide number (1-based)
     * @return The current slide number
     */
    int getCurrentSlideNumber() const;

    /**
     * @brief Gets the total number of slides
     * @return The slide count
     */
    int getTotalSlides() const;

    /**
     * @brief Gets the filename of this slideshow
     * @return The filename
     */
    std::string getFilename() const;

    /**
     * @brief Checks if the slideshow is empty
     * @return True if there are no slides
     */
    bool isEmpty() const;

    /**
     * @brief Gets a pointer to the current slide for modification
     * @return Pointer to current slide, or nullptr if no slides exist
     */
    Slide* getCurrentSlide();
};

#endif // SLIDESHOW_H
