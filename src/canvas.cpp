#include "vizlib/canvas.hpp"
#include <cmath>
#include <stdexcept>

namespace vizlib {

Canvas::Canvas(int width, int height, Color background)
    : width_(width), height_(height)
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Width and height must be positive integers.");
    }
    // creates width*height boxes and fills every one with the background colour
    pixels_.assign(width * height, background);
}

// stores the colour of a specific pixel in the vector. The vector is a
// one-dimensional list, so the 2D position (x, y) is converted to a box number.
void Canvas::set_pixel(int x, int y, Color c) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return;                       // 1. position is off the canvas: do nothing
    }
    pixels_[y * width_ + x] = c;      // 2. otherwise, put the colour in the right box
}

Color Canvas::get_pixel(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Pixel coordinates are out of bounds.");
    }
    return pixels_[y * width_ + x];   // otherwise, return the colour in the right box
}

int Canvas::width() const  { return width_; }
int Canvas::height() const { return height_; }

// fills every pixel inside the rectangle with the colour. Pixels that fall
// off the canvas are skipped by set_pixel.
void Canvas::fill_rect(Rect r, Color c) {
    const int x0 = static_cast<int>(std::lround(r.x));
    const int y0 = static_cast<int>(std::lround(r.y));
    const int x1 = static_cast<int>(std::lround(r.x + r.width));
    const int y1 = static_cast<int>(std::lround(r.y + r.height));
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            set_pixel(x, y, c);
        }
    }
}

}  // end of namespace vizlib