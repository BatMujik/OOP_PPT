# OOP Course Project: Multi-PPTX CLI Slideshow

This project is a **C++ command-line application** that simulates a slideshow for multiple presentations defined in text files. Each "presentation" file contains shapes positioned on slides, separated by `---`. The application demonstrates object-oriented programming principles, including classes for shapes, slides, slideshows, tokenizing input, and command parsing.

---

## **Features**

- **Load Multiple Presentations**: Accepts multiple text files as command-line arguments, parsing them into slides with shapes.
- **Slide Navigation**:
  - `next`: Move to the next slide in the current presentation.
  - `prev`: Move to the previous slide in the current presentation.
  - `show`: Display the current slide's shapes.
  - `goto <n>`: Jump to slide number `n` (1-based) in the current presentation.
  - `goto <filename> <n>`: Jump to slide number `n` in the specified presentation (supports filenames with or without `./` prefix).
- **Presentation Navigation**:
  - `nextfile`: Switch to the next loaded presentation and show its current slide.
  - `prevfile`: Switch to the previous loaded presentation and show its current slide.
- **Help and Exit**:
  - `help`: Display a list of available commands.
  - `exit`: Exit the program.
- **Input File Format**: Text files with lines like `ShapeName, x, y` for shapes on a slide, separated by `---` for new slides. Example:
  ```
  Circle, 10, 20
  Square, 30, 40
  ---
  Triangle, 50, 60
  Rectangle, 70, 80
  ```
- **Error Handling**: Graceful handling of invalid commands, slide numbers, filenames, and empty presentations.
- **Prompt**: Dynamic CLI prompt showing current presentation filename and slide position (e.g., `[pp1.txt, Slide 1/3] > `).

---

## **Project Structure**

```
oop_cp/
├── CMakeLists.txt          # CMake build configuration
├── include/                # Header files
│   ├── Shape.h             # Shape class definition
│   ├── Slide.h             # Slide class definition
│   ├── SlideShow.h         # SlideShow class definition
│   ├── Tokenizer.h         # Tokenizer utility class
│   └── CommandParser.h     # Command parser class
├── src/                    # Source files
│   ├── main.cpp            # Main application entry point
│   ├── Shape.cpp           # Shape implementation
│   ├── Slide.cpp           # Slide implementation
│   ├── SlideShow.cpp       # SlideShow implementation
│   ├── Tokenizer.cpp       # Tokenizer implementation
│   └── CommandParser.cpp   # Command parser implementation
├── pp1.txt                 # Sample presentation 1
├── pp2.txt                 # Sample presentation 2
├── pp3.txt                 # Sample presentation 3
└── README.md               # This file
```

---

## **Usage**

### **Build**

Use CMake to build the project:

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .
```

The executable will be created in `build/bin/slideshow` (or `build\bin\slideshow.exe` on Windows).

### **Run**

```bash
# Run with one or more presentation files
./bin/slideshow ../pp1.txt ../pp2.txt ../pp3.txt
```

On Windows:
```powershell
.\bin\slideshow.exe ..\pp1.txt ..\pp2.txt ..\pp3.txt
```

### **Commands**

Once the program starts, you'll see a prompt like `[pp1.txt, Slide 1/3] >`. Available commands:

- **`next`** - Move to the next slide in the current presentation
- **`prev`** - Move to the previous slide
- **`show`** - Display the current slide
- **`goto 2`** - Jump to slide 2 in the current presentation
- **`goto pp2.txt 1`** - Jump to slide 1 in pp2.txt
- **`nextfile`** - Switch to the next presentation
- **`prevfile`** - Switch to the previous presentation
- **`help`** - Show all available commands
- **`exit`** - Exit the program

### **Example Session**

```
[INFO] Loading presentations...
[INFO] Loaded 3 slide(s) from: pp1.txt
[INFO] Loaded 3 slide(s) from: pp2.txt
[INFO] Loaded 2 slide(s) from: pp3.txt
[INFO] 3 presentation(s) loaded successfully.

--- Slide 1/3 ---
  Circle at position (10, 20)
  Square at position (30, 40)
  Triangle at position (15, 25)

[pp1.txt, Slide 1/3] > next

--- Slide 2/3 ---
  Rectangle at position (50, 60)
  Hexagon at position (70, 80)

[pp1.txt, Slide 2/3] > goto 3

--- Slide 3/3 ---
  Star at position (100, 120)
  Diamond at position (90, 110)
  Pentagon at position (80, 100)

[pp1.txt, Slide 3/3] > nextfile
[INFO] Switched to: pp2.txt

--- Slide 1/3 ---
  Oval at position (200, 250)
  Trapezoid at position (180, 220)

[pp2.txt, Slide 1/3] > help

=== Available Commands ===
  next             - Move to the next slide in the current presentation
  prev             - Move to the previous slide in the current presentation
  show             - Display the current slide's shapes
  goto <n>         - Jump to slide number <n> (1-based) in the current presentation
  goto <file> <n>  - Jump to slide number <n> in the specified presentation
  nextfile         - Switch to the next loaded presentation
  prevfile         - Switch to the previous loaded presentation
  help             - Display this help message
  exit             - Exit the program
==========================

[pp2.txt, Slide 1/3] > exit
[INFO] Exiting slideshow. Goodbye!
```

> **Notes:**
> - Commands are case-insensitive.
> - Invalid commands or arguments show error messages (e.g., unknown command, invalid slide number).
> - If a filename is not found in `goto`, it lists available presentations for reference.

---

## **Development Notes**

### **Classes and Components**

- **`Shape`**: Represents a shape with name and position (x, y).
- **`Slide`**: Contains a vector of shapes and methods to add/show them.
- **`SlideShow`**: Manages slides for a single file, with navigation methods (next, prev, gotoSlide).
- **`Tokenizer`**: Splits lines by delimiter (e.g., comma) for parsing shape data, with trimming.
- **`CommandParser`**: Tokenizes and parses CLI input, validates commands, and normalizes paths.

### **Technologies Used**

- **Language**: C++17
- **Build System**: CMake 3.10+
- **Dependencies**: Standard C++ libraries only (`<iostream>`, `<fstream>`, `<string>`, `<vector>`, `<map>`, `<algorithm>`, `<cctype>`, `<sstream>`, `<memory>`)

### **Design Principles**

- **Object-Oriented Programming**: Clear separation of concerns with distinct classes for shapes, slides, and slideshows.
- **Memory Management**: Uses smart pointers (`std::unique_ptr`, `std::shared_ptr`) for automatic memory management.
- **Error Handling**: Graceful handling of file I/O errors, invalid input, and edge cases.
- **Path Normalization**: Filenames are normalized (lowercase, `/` separators, strip `./` prefix) for case-insensitive matching.

---

## **Sample Presentation Files**

Three sample presentation files are included:

**pp1.txt** (3 slides with geometric shapes)
**pp2.txt** (3 slides with natural objects)
**pp3.txt** (2 slides with 3D shapes)

You can create your own presentation files following the format:
```
ShapeName1, x1, y1
ShapeName2, x2, y2
---
ShapeName3, x3, y3
```

---

## **Future Improvements**

- **Real PPTX Support**: Integrate a library like OpenXML or libpptx to parse actual `.pptx` files.
- **Advanced Commands**: Add `search` for shapes, `edit` to modify slides interactively, or `save` to export changes.
- **GUI Integration**: Extend to a graphical interface using SFML, Qt, or SDL for visual rendering of shapes.
- **Shape Enhancements**: Add more properties (e.g., color, size, rotation) and rendering options.
- **Performance**: Optimize for large files with lazy loading of slides.
- **Testing**: Add unit tests using Google Test or Catch2 for classes like Tokenizer and CommandParser.
- **Animation Support**: Add transitions and animations between slides.
- **Export Features**: Export presentations to different formats (PDF, images, etc.).

---

## **Building and Testing**

### **Requirements**

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+, or newer)
- CMake 3.10 or higher

### **Build Instructions**

#### Linux/macOS:
```bash
mkdir build && cd build
cmake ..
make
./bin/slideshow ../pp1.txt ../pp2.txt ../pp3.txt
```

#### Windows (PowerShell):
```powershell
mkdir build; cd build
cmake ..
cmake --build .
.\bin\slideshow.exe ..\pp1.txt ..\pp2.txt ..\pp3.txt
```

### **Troubleshooting**

- **File not found errors**: Ensure presentation files are in the correct directory or use absolute paths.
- **Build errors**: Verify you have a C++17 compatible compiler and CMake 3.10+.
- **Runtime errors**: Check that presentation files follow the correct format (`ShapeName, x, y`).

---

## **Author**

**Artyom Hakobyan**  
OOP Course Project  
2025

---

## **License**

This project is created for educational purposes as part of an Object-Oriented Programming course.
