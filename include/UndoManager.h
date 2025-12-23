#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include "Slide.h"
#include <stack>
#include <string>

/**
 * @brief Represents a snapshot of a slide's state for undo/redo
 */
struct SlideSnapshot {
    int slideIndex;           // Which slide was modified
    Slide slideState;         // Copy of the slide at that moment
    std::string description;  // What action was performed
    
    SlideSnapshot(int idx, const Slide& slide, const std::string& desc)
        : slideIndex(idx), slideState(slide), description(desc) {}
};

/**
 * @brief Manages undo/redo history for slideshow modifications
 * Implements the Memento pattern for state management
 */
class UndoManager {
private:
    std::stack<SlideSnapshot> undoStack;
    std::stack<SlideSnapshot> redoStack;
    size_t maxHistorySize;

public:
    /**
     * @brief Constructs an UndoManager with optional max history size
     * @param maxSize Maximum number of actions to remember (0 = unlimited)
     */
    UndoManager(size_t maxSize = 50);

    /**
     * @brief Saves the current state before a modification
     * @param slideIndex The index of the slide being modified
     * @param slide The current state of the slide
     * @param actionDescription Description of the action about to be performed
     */
    void saveState(int slideIndex, const Slide& slide, const std::string& actionDescription);

    /**
     * @brief Checks if undo is available
     */
    bool canUndo() const;

    /**
     * @brief Checks if redo is available
     */
    bool canRedo() const;

    /**
     * @brief Gets the description of the action that would be undone
     */
    std::string getUndoDescription() const;

    /**
     * @brief Gets the description of the action that would be redone
     */
    std::string getRedoDescription() const;

    /**
     * @brief Performs undo operation
     * @param currentSlide The current slide state (to save for redo)
     * @return The snapshot to restore, or nullptr if can't undo
     */
    SlideSnapshot* undo(const Slide& currentSlide);

    /**
     * @brief Performs redo operation
     * @param currentSlide The current slide state (to save for undo)
     * @return The snapshot to restore, or nullptr if can't redo
     */
    SlideSnapshot* redo(const Slide& currentSlide);

    /**
     * @brief Clears the redo stack (called when a new action is performed)
     */
    void clearRedo();

    /**
     * @brief Clears all history
     */
    void clear();

    /**
     * @brief Gets the number of actions in undo history
     */
    size_t getUndoCount() const;

    /**
     * @brief Gets the number of actions in redo history
     */
    size_t getRedoCount() const;

private:
    // Temporary storage for returning snapshots
    SlideSnapshot tempSnapshot{0, Slide(), ""};
};

#endif // UNDOMANAGER_H

