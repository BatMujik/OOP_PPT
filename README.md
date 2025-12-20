# SlideShow Advanced - Visual Presentation System

<p align="center">
  <strong>🎨 Advanced Visual Presentation System</strong><br>
  <em>Version 2.0 - OOP Course Project</em>
</p>

---

## ✨ Features Overview

This is an **advanced C++ command-line slideshow application** that demonstrates professional OOP design patterns and provides rich visual features:

### 🎯 Core Features

| Feature | Description |
|---------|-------------|
| **🎨 Visual Rendering** | Real-time ASCII art visualization of shapes in the terminal |
| **🌈 Full Color Support** | 20+ named colors with ANSI terminal color output |
| **📤 Image Export** | Export slides to **BMP** (Windows native) and **SVG** (vector graphics) |
| **🔷 Rich Shape Properties** | Color, size, position, border width, fill options |
| **🏭 Factory Pattern** | Professional shape creation using the Factory design pattern |
| **✏️ Interactive Editing** | Add, edit, delete shapes with intuitive interface |
| **📁 Multi-Presentation** | Load and navigate multiple presentations |

### 🎨 Supported Shapes

| Shape | Icon | Default Color |
|-------|------|---------------|
| Circle | ● | Red |
| Rectangle | ▬ | Blue |
| Square | ■ | Green |
| Triangle | ▲ | Yellow |
| Diamond | ◆ | Cyan |
| Star | ★ | Gold |
| Hexagon | ⬡ | Purple |
| Pentagon | ⬠ | Teal |
| Oval | ⬭ | Pink |
| Trapezoid | ⏢ | Coral |
| Rhombus | ◇ | Violet |
| Line | ─ | White |
| Text | T | White |

### 🌈 Available Colors

```
Red, Green, Blue, Yellow, Cyan, Magenta, Orange, Purple, Pink, 
White, Black, Gray, Brown, Gold, Silver, Navy, Teal, Lime, Coral, Violet
```

---

## 📁 Project Structure

```
oop_cp/
├── CMakeLists.txt           # CMake build configuration
├── include/                  # Header files
│   ├── Shape.h              # Polymorphic shape hierarchy
│   ├── Slide.h              # Slide container with rendering
│   ├── SlideShow.h          # Presentation manager
│   ├── Canvas.h             # Pixel canvas for rendering
│   ├── Color.h              # RGB color with ANSI support
│   ├── ShapeFactory.h       # Factory pattern implementation
│   ├── Tokenizer.h          # String parsing utilities
│   └── CommandParser.h      # Command parsing
├── src/                      # Source files
│   ├── main.cpp             # Application entry point
│   ├── Shape.cpp            # Shape implementations
│   ├── Slide.cpp            # Slide implementation
│   ├── SlideShow.cpp        # SlideShow implementation
│   ├── Canvas.cpp           # Canvas rendering & export
│   ├── Color.cpp            # Color implementation
│   ├── ShapeFactory.cpp     # Factory implementation
│   ├── Tokenizer.cpp        # Tokenizer implementation
│   └── CommandParser.cpp    # Command parser implementation
├── pp1.txt                   # Sample presentation 1
├── pp2.txt                   # Sample presentation 2
├── pp3.txt                   # Sample presentation 3
└── README.md                 # This file
```

---

## 🚀 Quick Start

### Build

```powershell
# Create and enter build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .
```

### Run

```powershell
# Run with sample presentations
.\bin\slideshow.exe ..\pp1.txt ..\pp2.txt ..\pp3.txt
```

---

## 📖 Commands Reference

### Navigation Commands

| Command | Shortcut | Description |
|---------|----------|-------------|
| `next` | `n` | Move to next slide |
| `prev` | `p` | Move to previous slide |
| `show` | `s` | Display current slide |
| `goto <n>` | `g <n>` | Jump to slide number n |
| `goto <file> <n>` | - | Jump to slide n in another file |
| `nextfile` | `nf` | Switch to next presentation |
| `prevfile` | `pf` | Switch to previous presentation |

### Shape Manipulation

| Command | Shortcut | Description |
|---------|----------|-------------|
| `add` | `a` | Add new shape (interactive wizard) |
| `delete <n>` | `del <n>` | Delete shape by index |
| `edit <n>` | `e <n>` | Edit shape properties |
| `list` | `ls` | List all shapes on current slide |
| `clear` | - | Remove all shapes from slide |

### View Modes

| Command | Shortcut | Description |
|---------|----------|-------------|
| `visual` | `v` | Switch to ASCII art visual mode |
| `text` | `t` | Switch to text list mode |

### Export

| Command | Description |
|---------|-------------|
| `export slide.bmp` | Export current slide as BMP image |
| `export slide.svg` | Export current slide as SVG vector |
| `exportall prefix` | Export all slides (prefix_1.bmp, prefix_1.svg, etc.) |

### File Operations

| Command | Description |
|---------|-------------|
| `new <file>` | Create a **new empty presentation** (e.g., `new pp5.txt`) |
| `save` | Save current presentation to its file |
| `saveas <file>` | Save current presentation to a new file (copies all slides) |

### Utilities

| Command | Shortcut | Description |
|---------|----------|-------------|
| `shapes` | - | Show available shape types |
| `colors` | - | Show available colors |
| `help` | `h`, `?` | Show help message |
| `exit` | `q` | Exit program |

---

## 📝 File Format

### Basic Format (backward compatible)
```
ShapeName, x, y
```

### Extended Format
```
ShapeName, x, y, width, height, fillColor, borderColor, borderWidth, filled
```

### Example
```
Circle, 50, 30, 60, 60, Red, White, 2, true
Rectangle, 120, 40, 80, 50, Blue, Cyan, 3, true
Star, 220, 25, 70, 70, Gold, Orange, 2, true
---
Triangle, 40, 30, 60, 50, Yellow, White, 2, true
Diamond, 150, 35, 45, 60, Purple, Pink, 2, true
```

---

## 💡 Example Session

```
╔═══════════════════════════════════════════════════════════════════╗
║   ███████╗██╗     ██╗██████╗ ███████╗███████╗██╗  ██╗ ██████╗ ██╗    ██║
║              Advanced Visual Presentation System                   ║
╚═══════════════════════════════════════════════════════════════════╝

[OK] Loaded 3 slide(s) from: pp1.txt
[OK] 3 presentation(s) loaded. Type help for commands.

╔═══════════════════════════════════════════════════════════════╗
║  Slide 1/3                                                    ║
╚═══════════════════════════════════════════════════════════════╝

  ● Circle at (20, 20) [50x50] Color: Red
  ■ Square at (100, 30) [40x40] Color: Green
  ▲ Triangle at (180, 20) [60x50] Color: Yellow

[pp1.txt, Slide 1/3] ▶ visual

[OK] Switched to visual mode.

══════════════ Slide 1/3 ══════════════
+--------------------------------------------------------+
|                                                        |
|    ████                    ████                        |
|   ██████                  ██████          ▲▲           |
|   ██████                  ██████         ▲▲▲▲          |
|    ████                    ████         ▲▲▲▲▲▲         |
|                                                        |
+--------------------------------------------------------+

[pp1.txt, Slide 1/3] ▶ export myslide.bmp
[OK] Exported to: myslide.bmp

[pp1.txt, Slide 1/3] ▶ add

  ═══════════ Add New Shape ═══════════
   1. ● Circle
   2. ▬ Rectangle
   3. ■ Square
   ...
  ═════════════════════════════════════════
  Enter shape number: 6
  Enter position (x y): 150 80
  Enter size (width height) or press Enter for default: 40 40
  Enter color name or press Enter for default: gold
[OK] Added Star at (150, 80)

[pp1.txt, Slide 1/3] ▶ exit
[INFO] Exiting slideshow. Goodbye!
```

---

## 🏗️ OOP Design Patterns

### 1. **Polymorphism & Inheritance**
- Abstract `Shape` base class with virtual methods
- Concrete shape classes (Circle, Rectangle, Star, etc.)
- Each shape implements `render()`, `toSVG()`, `clone()`

### 2. **Factory Pattern**
- `ShapeFactory` creates shapes by name
- Centralized shape creation logic
- Easy to add new shape types

### 3. **Command Pattern**
- `CommandParser` processes user input
- Clean separation of parsing and execution
- Extensible command structure

### 4. **Smart Pointers**
- `std::unique_ptr` for ownership
- `std::shared_ptr` for shared access
- No manual memory management

---

## 🔧 Technical Details

### Requirements
- **C++17** compatible compiler
- **CMake 3.10+**
- Windows/Linux/macOS

### Image Export Formats

| Format | Description | How to View |
|--------|-------------|-------------|
| **BMP** | Windows Bitmap | Double-click on Windows |
| **SVG** | Scalable Vector Graphics | Open in any browser |

### Canvas Rendering
- 320x180 pixel canvas (16:9 aspect ratio)
- ASCII art scaled for terminal display
- Full RGB color support via ANSI escape codes

---

## 👤 Author

**Artyom Hakobyan**  
OOP Course Project  
2025

---

## 📄 License

This project is created for educational purposes as part of an Object-Oriented Programming course.