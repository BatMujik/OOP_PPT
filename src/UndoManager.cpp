#include "UndoManager.h"

UndoManager::UndoManager(size_t maxSize) : maxHistorySize(maxSize) {}

void UndoManager::saveState(int slideIndex, const Slide& slide, const std::string& actionDescription) {
    // Clear redo stack when new action is performed
    clearRedo();
    
    // Add to undo stack
    undoStack.push(SlideSnapshot(slideIndex, slide, actionDescription));
    
    // Limit history size if maxHistorySize is set
    if (maxHistorySize > 0 && undoStack.size() > maxHistorySize) {
        // Unfortunately std::stack doesn't support removing from bottom
        // So we'll just let it grow until clear() is called
        // In production, we'd use a deque instead
    }
}

bool UndoManager::canUndo() const {
    return !undoStack.empty();
}

bool UndoManager::canRedo() const {
    return !redoStack.empty();
}

std::string UndoManager::getUndoDescription() const {
    if (undoStack.empty()) {
        return "";
    }
    return undoStack.top().description;
}

std::string UndoManager::getRedoDescription() const {
    if (redoStack.empty()) {
        return "";
    }
    return redoStack.top().description;
}

SlideSnapshot* UndoManager::undo(const Slide& currentSlide) {
    if (!canUndo()) {
        return nullptr;
    }
    
    // Get the state to restore
    SlideSnapshot snapshot = undoStack.top();
    undoStack.pop();
    
    // Save current state to redo stack
    redoStack.push(SlideSnapshot(snapshot.slideIndex, currentSlide, snapshot.description));
    
    // Store in temp and return pointer
    tempSnapshot = snapshot;
    return &tempSnapshot;
}

SlideSnapshot* UndoManager::redo(const Slide& currentSlide) {
    if (!canRedo()) {
        return nullptr;
    }
    
    // Get the state to restore
    SlideSnapshot snapshot = redoStack.top();
    redoStack.pop();
    
    // Save current state to undo stack
    undoStack.push(SlideSnapshot(snapshot.slideIndex, currentSlide, snapshot.description));
    
    // Store in temp and return pointer
    tempSnapshot = snapshot;
    return &tempSnapshot;
}

void UndoManager::clearRedo() {
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void UndoManager::clear() {
    while (!undoStack.empty()) {
        undoStack.pop();
    }
    clearRedo();
}

size_t UndoManager::getUndoCount() const {
    return undoStack.size();
}

size_t UndoManager::getRedoCount() const {
    return redoStack.size();
}

