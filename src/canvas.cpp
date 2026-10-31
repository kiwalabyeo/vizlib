#include "vizlib/canvas.hpp"
#include <cmath>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <string>

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

// namespace {
// // Colours a square of side `thickness` around (x, y). Thickness 1 = one pixel.
// void stamp(Canvas& canvas, int x, int y, int thickness, Color c) {
//     int half = thickness / 2;                      // integer division, e.g. 3/2 = 1
//     for (int dy = 0; dy < thickness; ++dy)
//         for (int dx = 0; dx < thickness; ++dx)
//             canvas.set_pixel(x - half + dx, y - half + dy, c);  // set_pixel ignores off-canvas
// }
// }  // end unnamed namespace: only this file can use stamp()

namespace {

constexpr double kMaxCoord = 1e6;    // biggest allowed coordinate or radius
constexpr int kMaxThickness = 1000;  // biggest allowed line thickness

// Throws if v is NaN, infinity, or absurdly large.
void check_coord(double v, const char* name) {
    if (!std::isfinite(v) || std::fabs(v) > kMaxCoord) {
        throw std::invalid_argument(std::string(name) + " must be a finite number within +/-1000000");
    }
}

// Colours a square of side `thickness` around (x, y). Thickness 1 = one pixel.
void stamp(Canvas& canvas, int x, int y, int thickness, Color c) {
    int half = thickness / 2; // integer division, e.g. 3/2 = 1
    for (int dy = 0; dy < thickness; ++dy)
        for (int dx = 0; dx < thickness; ++dx)
            canvas.set_pixel(x - half + dx, y - half + dy, c); // set_pixel ignores off-canvas
}

}  // end unnamed namespace: only this file can use stamp()

// Bresenham's line algorithm (Bresenham, 1965): uses only integers, no rounding errors.
void Canvas::draw_line(Point from, Point to, Color c, int thickness) {
    // if (thickness < 1) throw std::invalid_argument("thickness must be at least 1");
    if (thickness < 1 || thickness > kMaxThickness) {
        throw std::invalid_argument("thickness must be between 1 and 1000");
    }
    check_coord(from.x, "from.x");  check_coord(from.y, "from.y");
    check_coord(to.x, "to.x");      check_coord(to.y, "to.y");

    int x0 = static_cast<int>(std::lround(from.x));  // doubles -> nearest whole pixel
    int y0 = static_cast<int>(std::lround(from.y));
    int x1 = static_cast<int>(std::lround(to.x));
    int y1 = static_cast<int>(std::lround(to.y));

    // If both ends are past the same edge of the canvas, no part of the line can be visible, so skip it instead of walking the whole invisible line.
    const int pad = thickness;
    if ((x0 < -pad && x1 < -pad) || (x0 >= width_ + pad && x1 >= width_ + pad) ||
        (y0 < -pad && y1 < -pad) || (y0 >= height_ + pad && y1 >= height_ + pad)) {
        return;
    }

    int dx = std::abs(x1 - x0);          // how far we go across
    int dy = -std::abs(y1 - y0);         // how far we go down, stored NEGATIVE (trick that keeps the maths simple)
    int sx = (x0 < x1) ? 1 : -1;         // step direction in x: right or left
    int sy = (y0 < y1) ? 1 : -1;         // step direction in y: down or up
    int err = dx + dy;                   // error term: how far we are from the true line

    while (true) {
        stamp(*this, x0, y0, thickness, c);        // colour the current pixel
        if (x0 == x1 && y0 == y1) break;           // reached the end
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }     // error says: step in x
        if (e2 <= dx) { err += dx; y0 += sy; }     // error says: step in y (both can happen = diagonal)
    }
}

// void Canvas::draw_rect(Rect r, Color c) {
//     if (r.width <= 0 || r.height <= 0) throw std::invalid_argument("Rect must have positive width and height");
//     double left = r.x, top = r.y;
//     double right = r.x + r.width - 1;     // -1 because the last pixel is width-1 away from the first
//     double bottom = r.y + r.height - 1;
//     draw_line({left, top},     {right, top},    c);   // top edge
//     draw_line({right, top},    {right, bottom}, c);   // right edge
//     draw_line({right, bottom}, {left, bottom},  c);   // bottom edge
//     draw_line({left, bottom},  {left, top},     c);   // left edge
// }

// Outline of a rectangle. An empty rectangle (width or height <= 0) draws nothing.
void Canvas::draw_rect(Rect r, Color c) {
    check_coord(r.x, "rect.x");          check_coord(r.y, "rect.y");
    check_coord(r.width, "rect.width");  check_coord(r.height, "rect.height");
    if (r.width <= 0 || r.height <= 0) return;
    double left = r.x, top = r.y;
    double right = r.x + r.width - 1;     // -1 because the last pixel is width-1 away from the first
    double bottom = r.y + r.height - 1;
    draw_line({left, top},     {right, top},    c);   // top edge
    draw_line({right, top},    {right, bottom}, c);   // right edge
    draw_line({right, bottom}, {left, bottom},  c);   // bottom edge
    draw_line({left, bottom},  {left, top},     c);   // left edge
}

// fills every pixel inside the rectangle with the colour. The loop limits are
// clamped to the canvas, so the part that is off the canvas is never visited.
// Solid rectangle. An empty rectangle draws nothing (a histogram bar of height 0 is valid).
void Canvas::fill_rect(Rect r, Color c) {
    check_coord(r.x, "rect.x");          check_coord(r.y, "rect.y");
    check_coord(r.width, "rect.width");  check_coord(r.height, "rect.height");
    if (r.width <= 0 || r.height <= 0) return;
    const int x0 = std::max(0, static_cast<int>(std::lround(r.x)));
    const int y0 = std::max(0, static_cast<int>(std::lround(r.y)));
    const int x1 = std::min(width_,  static_cast<int>(std::lround(r.x + r.width)));
    const int y1 = std::min(height_, static_cast<int>(std::lround(r.y + r.height)));
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            set_pixel(x, y, c);
        }
    }
}

// Midpoint circle algorithm: walk one eighth of the circle, mirror it 8 ways.
void Canvas::draw_circle(Point center, double radius, Color c) {
    if (radius < 0) throw std::invalid_argument("radius must not be negative");
    check_coord(center.x, "center.x");  check_coord(center.y, "center.y");
    check_coord(radius, "radius");
    int cx = static_cast<int>(std::lround(center.x));
    int cy = static_cast<int>(std::lround(center.y));
    int x = static_cast<int>(std::lround(radius));
    int y = 0;
    int err = 1 - x;
    while (x >= y) {
        set_pixel(cx + x, cy + y, c);  set_pixel(cx - x, cy + y, c);
        set_pixel(cx + x, cy - y, c);  set_pixel(cx - x, cy - y, c);
        set_pixel(cx + y, cy + x, c);  set_pixel(cx - y, cy + x, c);
        set_pixel(cx + y, cy - x, c);  set_pixel(cx - y, cy - x, c);
        ++y;
        if (err <= 0) { err += 2 * y + 1; }
        else          { --x; err += 2 * (y - x) + 1; }
    }
}

void Canvas::fill_circle(Point center, double radius, Color c) {
    if (radius < 0) throw std::invalid_argument("radius must not be negative");
    check_coord(center.x, "center.x");
    check_coord(center.y, "center.y");
    check_coord(radius, "radius");
    int cx = static_cast<int>(std::lround(center.x));
    int cy = static_cast<int>(std::lround(center.y));
    int r = static_cast<int>(std::lround(radius));
    const int first = std::max(-r, -cy);                 // top row that is on the canvas
    const int last  = std::min(r, height_ - 1 - cy);     // bottom row that is on the canvas
    for (int dy = first; dy <= last; ++dy) {
        long long rr = static_cast<long long>(r) * r - static_cast<long long>(dy) * dy;
        int half = static_cast<int>(std::lround(std::sqrt(static_cast<double>(rr))));
        int xa = std::max(cx - half, 0);                 // clamp each strip to the canvas
        int xb = std::min(cx + half, width_ - 1);
        for (int x = xa; x <= xb; ++x) set_pixel(x, cy + dy, c);
    }
}
}  // end of namespace vizlib