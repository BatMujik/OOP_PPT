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
    bool modified;

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
     * @brief Creates and adds a new empty slide
     * @return Index of the new slide
     */
    int addNewSlide();

    /**
     * @brief Removes a slide by index (1-based)
     * @return True if successful
     */
    bool removeSlide(int slideNumber);

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
     * @brief Displays the current slide in text format
     */
    void show() const;

    /**
     * @brief Displays the current slide with colored text
     */
    void showColored() const;

    /**
     * @brief Displays the current slide as ASCII art
     */
    void showVisual() const;

    /**
     * @brief Jumps to a specific slide (1-based index)
     * @param slideNumber The slide number to jump to
     * @return True if successful, false if slide number is invalid
     */
    bool gotoSlide(int slideNumber);

    /**
     * @brief Gets the current slide number (1-based)
     */
    int getCurrentSlideNumber() const;

    /**
     * @brief Gets the total number of slides
     */
    int getTotalSlides() const;

    /**
     * @brief Gets the filename of this slideshow
     */
    std::string getFilename() const;

    /**
     * @brief Checks if the slideshow is empty
     */
    bool isEmpty() const;

    /**
     * @brief Gets a pointer to the current slide for modification
     */
    Slide* getCurrentSlide();

    /**
     * @brief Gets a const pointer to the current slide
     */
    const Slide* getCurrentSlide() const;

    /**
     * @brief Exports current slide to BMP
     */
    bool exportCurrentSlideBMP(const std::string& filename) const;

    /**
     * @brief Exports current slide to SVG
     */
    bool exportCurrentSlideSVG(const std::string& filename) const;

    /**
     * @brief Exports current slide to JSON
     */
    bool exportCurrentSlideJSON(const std::string& filename) const;

    /**
     * @brief Exports all slides to BMP files
     * @param prefix Filename prefix (e.g., "slide" creates slide_1.bmp, slide_2.bmp, etc.)
     */
    void exportAllSlidesBMP(const std::string& prefix) const;

    /**
     * @brief Exports all slides to SVG files
     */
    void exportAllSlidesSVG(const std::string& prefix) const;

    /**
     * @brief Exports all slides to JSON files
     */
    void exportAllSlidesJSON(const std::string& prefix) const;

    /**
     * @brief Exports entire slideshow to a single JSON file
     */
    bool exportSlideshowJSON(const std::string& filename) const;

    /**
     * @brief Checks if slideshow has been modified
     */
    bool isModified() const { return modified; }

    /**
     * @brief Marks slideshow as modified
     */
    void setModified(bool m) { modified = m; }

    /**
     * @brief Saves the slideshow to a file
     * @param filename Optional new filename (uses current if empty)
     * @return True if saved successfully
     */
    bool save(const std::string& newFilename = "");

    /**
     * @brief Sets a new filename for the slideshow
     */
    void setFilename(const std::string& name) { filename = name; }
};

#endif // SLIDESHOW_H
