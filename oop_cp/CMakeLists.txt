cmake_minimum_required(VERSION 3.10)
project(MultiPPTX_Slideshow)

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
)

# Header files
set(HEADERS
    include/Shape.h
    include/Slide.h
    include/SlideShow.h
    include/Tokenizer.h
    include/CommandParser.h
)

# Create executable
add_executable(slideshow ${SOURCES} ${HEADERS})

# Set output directory
set_target_properties(slideshow PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
