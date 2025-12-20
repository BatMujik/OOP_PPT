cmake_minimum_required(VERSION 3.10)
project(SlideShow_Advanced VERSION 2.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Shape.cpp
    src/Slide.cpp
    src/SlideShow.cpp
    src/Tokenizer.cpp
    src/CommandParser.cpp
    src/Color.cpp
    src/Canvas.cpp
    src/ShapeFactory.cpp
)

# Header files
set(HEADERS
    include/Shape.h
    include/Slide.h
    include/SlideShow.h
    include/Tokenizer.h
    include/CommandParser.h
    include/Color.h
    include/Canvas.h
    include/ShapeFactory.h
)

# Create executable
add_executable(slideshow ${SOURCES} ${HEADERS})

# Set output directory
set_target_properties(slideshow PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Platform-specific settings
if(WIN32)
    # Enable Unicode and ANSI colors on Windows
    target_compile_definitions(slideshow PRIVATE _UNICODE UNICODE)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(slideshow PRIVATE /W4)
else()
    target_compile_options(slideshow PRIVATE -Wall -Wextra -Wpedantic)
endif()
