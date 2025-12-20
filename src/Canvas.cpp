#include "Canvas.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdint>

Canvas::Canvas(int width, int height, const Color& bgColor)
    : width(width), height(height), backgroundColor(bgColor) {
    pixels.resize(height, std::vector<Color>(width, bgColor));
    
    // ASCII buffer is scaled down (each ASCII char represents multiple pixels)
    int asciiWidth = width / 2;
    int asciiHeight = height / 4;
    asciiBuffer.resize(asciiHeight, std::vector<char>(asciiWidth, ' '));
    asciiFgColors.resize(asciiHeight, std::vector<Color>(asciiWidth, bgColor));
}

void Canvas::clear() {
    for (auto& row : pixels) {
        std::fill(row.begin(), row.end(), backgroundColor);
    }
    for (auto& row : asciiBuffer) {
        std::fill(row.begin(), row.end(), ' ');
    }
    for (auto& row : asciiFgColors) {
        std::fill(row.begin(), row.end(), backgroundColor);
    }
}

void Canvas::setPixel(int x, int y, const Color& color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[y][x] = color;
        
        // Also update ASCII buffer
        int ax = x / 2;
        int ay = y / 4;
        if (ax < static_cast<int>(asciiBuffer[0].size()) && ay < static_cast<int>(asciiBuffer.size())) {
            asciiBuffer[ay][ax] = '#';
            asciiFgColors[ay][ax] = color;
        }
    }
}

Color Canvas::getPixel(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return pixels[y][x];
    }
    return backgroundColor;
}

void Canvas::setAscii(int x, int y, char ch, const Color& color) {
    int ax = x / 2;
    int ay = y / 4;
    if (ax >= 0 && ax < static_cast<int>(asciiBuffer[0].size()) && 
        ay >= 0 && ay < static_cast<int>(asciiBuffer.size())) {
        asciiBuffer[ay][ax] = ch;
        asciiFgColors[ay][ax] = color;
    }
}

void Canvas::fillRect(int x, int y, int w, int h, const Color& color) {
    for (int py = y; py < y + h; ++py) {
        for (int px = x; px < x + w; ++px) {
            setPixel(px, py, color);
        }
    }
}

void Canvas::drawRect(int x, int y, int w, int h, const Color& color, int thickness) {
    // Top and bottom edges
    for (int t = 0; t < thickness; ++t) {
        for (int px = x; px < x + w; ++px) {
            setPixel(px, y + t, color);
            setPixel(px, y + h - 1 - t, color);
        }
    }
    // Left and right edges
    for (int t = 0; t < thickness; ++t) {
        for (int py = y; py < y + h; ++py) {
            setPixel(x + t, py, color);
            setPixel(x + w - 1 - t, py, color);
        }
    }
}

void Canvas::fillCircle(int cx, int cy, int radius, const Color& color) {
    for (int py = -radius; py <= radius; ++py) {
        for (int px = -radius; px <= radius; ++px) {
            if (px * px + py * py <= radius * radius) {
                setPixel(cx + px, cy + py, color);
            }
        }
    }
}

void Canvas::drawCircle(int cx, int cy, int radius, const Color& color, int thickness) {
    for (int py = -radius - thickness; py <= radius + thickness; ++py) {
        for (int px = -radius - thickness; px <= radius + thickness; ++px) {
            int distSq = px * px + py * py;
            int outerR = radius;
            int innerR = radius - thickness;
            if (distSq <= outerR * outerR && distSq >= innerR * innerR) {
                setPixel(cx + px, cy + py, color);
            }
        }
    }
}

void Canvas::fillEllipse(int cx, int cy, int rx, int ry, const Color& color) {
    for (int py = -ry; py <= ry; ++py) {
        for (int px = -rx; px <= rx; ++px) {
            double test = (double)(px * px) / (rx * rx) + (double)(py * py) / (ry * ry);
            if (test <= 1.0) {
                setPixel(cx + px, cy + py, color);
            }
        }
    }
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, const Color& color, int thickness) {
    // Bresenham's line algorithm with thickness
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        // Draw thick point
        for (int ty = -thickness / 2; ty <= thickness / 2; ++ty) {
            for (int tx = -thickness / 2; tx <= thickness / 2; ++tx) {
                setPixel(x1 + tx, y1 + ty, color);
            }
        }
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void Canvas::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color& color) {
    // Sort vertices by y-coordinate
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
    if (y1 > y3) { std::swap(x1, x3); std::swap(y1, y3); }
    if (y2 > y3) { std::swap(x2, x3); std::swap(y2, y3); }
    
    auto interpolate = [](int y, int y1, int y2, int x1, int x2) -> int {
        if (y2 == y1) return x1;
        return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    };
    
    for (int y = y1; y <= y3; ++y) {
        int xa, xb;
        if (y < y2) {
            xa = interpolate(y, y1, y2, x1, x2);
            xb = interpolate(y, y1, y3, x1, x3);
        } else {
            xa = interpolate(y, y2, y3, x2, x3);
            xb = interpolate(y, y1, y3, x1, x3);
        }
        
        if (xa > xb) std::swap(xa, xb);
        for (int x = xa; x <= xb; ++x) {
            setPixel(x, y, color);
        }
    }
}

bool Canvas::pointInPolygon(int px, int py, const std::vector<std::pair<int, int>>& points) const {
    int n = static_cast<int>(points.size());
    bool inside = false;
    
    for (int i = 0, j = n - 1; i < n; j = i++) {
        int xi = points[i].first, yi = points[i].second;
        int xj = points[j].first, yj = points[j].second;
        
        if (((yi > py) != (yj > py)) &&
            (px < (xj - xi) * (py - yi) / (yj - yi) + xi)) {
            inside = !inside;
        }
    }
    
    return inside;
}

void Canvas::fillPolygon(const std::vector<std::pair<int, int>>& points, const Color& color) {
    if (points.empty()) return;
    
    // Find bounding box
    int minX = points[0].first, maxX = points[0].first;
    int minY = points[0].second, maxY = points[0].second;
    
    for (const auto& p : points) {
        minX = std::min(minX, p.first);
        maxX = std::max(maxX, p.first);
        minY = std::min(minY, p.second);
        maxY = std::max(maxY, p.second);
    }
    
    // Fill using point-in-polygon test
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (pointInPolygon(x, y, points)) {
                setPixel(x, y, color);
            }
        }
    }
}

void Canvas::drawPolygon(const std::vector<std::pair<int, int>>& points, const Color& color, int thickness) {
    int n = static_cast<int>(points.size());
    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n;
        drawLine(points[i].first, points[i].second,
                 points[next].first, points[next].second, color, thickness);
    }
}

bool Canvas::exportBMP(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // BMP file format - 24-bit color, no compression
    int rowSize = ((width * 3 + 3) / 4) * 4;  // Rows must be 4-byte aligned
    int imageSize = rowSize * height;
    int fileSize = 54 + imageSize;  // Header (54 bytes) + image data
    
    // BMP File Header (14 bytes)
    uint8_t bmpHeader[14] = {
        'B', 'M',                           // Signature
        static_cast<uint8_t>(fileSize),      // File size (4 bytes)
        static_cast<uint8_t>(fileSize >> 8),
        static_cast<uint8_t>(fileSize >> 16),
        static_cast<uint8_t>(fileSize >> 24),
        0, 0, 0, 0,                          // Reserved
        54, 0, 0, 0                          // Offset to pixel data
    };
    file.write(reinterpret_cast<char*>(bmpHeader), 14);
    
    // DIB Header (40 bytes - BITMAPINFOHEADER)
    uint8_t dibHeader[40] = {
        40, 0, 0, 0,                         // DIB header size
        static_cast<uint8_t>(width),          // Width (4 bytes)
        static_cast<uint8_t>(width >> 8),
        static_cast<uint8_t>(width >> 16),
        static_cast<uint8_t>(width >> 24),
        static_cast<uint8_t>(height),         // Height (4 bytes)
        static_cast<uint8_t>(height >> 8),
        static_cast<uint8_t>(height >> 16),
        static_cast<uint8_t>(height >> 24),
        1, 0,                                // Color planes (1)
        24, 0,                               // Bits per pixel (24)
        0, 0, 0, 0,                          // Compression (none)
        static_cast<uint8_t>(imageSize),      // Image size (4 bytes)
        static_cast<uint8_t>(imageSize >> 8),
        static_cast<uint8_t>(imageSize >> 16),
        static_cast<uint8_t>(imageSize >> 24),
        0, 0, 0, 0,                          // X pixels per meter
        0, 0, 0, 0,                          // Y pixels per meter
        0, 0, 0, 0,                          // Colors in color table
        0, 0, 0, 0                           // Important colors
    };
    file.write(reinterpret_cast<char*>(dibHeader), 40);
    
    // Pixel data (bottom-up, BGR format)
    std::vector<uint8_t> row(rowSize, 0);
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            const Color& c = pixels[y][x];
            row[x * 3 + 0] = c.b;  // Blue
            row[x * 3 + 1] = c.g;  // Green
            row[x * 3 + 2] = c.r;  // Red
        }
        file.write(reinterpret_cast<char*>(row.data()), rowSize);
    }
    
    file.close();
    return true;
}

std::string Canvas::toAsciiArt() const {
    std::string result;
    
    // Top border
    result += Color(100, 100, 120).toAnsiFg();
    result += "+" + std::string(asciiBuffer[0].size(), '-') + "+\n";
    
    for (size_t y = 0; y < asciiBuffer.size(); ++y) {
        result += Color(100, 100, 120).toAnsiFg() + "|";
        for (size_t x = 0; x < asciiBuffer[y].size(); ++x) {
            char ch = asciiBuffer[y][x];
            if (ch != ' ') {
                result += asciiFgColors[y][x].toAnsiFg();
                // Use block characters for better visual
                result += "\u2588";  // Full block
            } else {
                result += backgroundColor.toAnsiFg() + " ";
            }
        }
        result += Color(100, 100, 120).toAnsiFg() + "|\n";
    }
    
    // Bottom border
    result += Color(100, 100, 120).toAnsiFg();
    result += "+" + std::string(asciiBuffer[0].size(), '-') + "+";
    result += Color::resetAnsi() + "\n";
    
    return result;
}

void Canvas::displayAscii() const {
    std::cout << toAsciiArt();
}

