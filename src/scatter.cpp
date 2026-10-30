#include "vizlib/scatter.hpp"

#include <cmath>
#include <utility>

namespace vizlib {

namespace detail {

// Adds 5% padding to each side so no dot sits on the border.
DataBounds pad_bounds(DataBounds b) {
    for (auto [lo, hi] : {std::pair{&b.x_min, &b.x_max}, std::pair{&b.y_min, &b.y_max}}) {
        double span = *hi - *lo;
        if (span == 0) {
            // all values are equal: give the range a width of 1 so we never
            // divide by zero, and the dots land in the middle
            *lo -= 0.5;
            *hi += 0.5;
            span = 1.0;
        }
        *lo -= 0.05 * span;
        *hi += 0.05 * span;
    }
    return b;
}

// Draws a filled circle by colouring every pixel whose distance from the
// centre is at most `radius`.
void draw_dot(Canvas& canvas, Point center, double radius, Color c) {
    const int cx = static_cast<int>(std::lround(center.x));
    const int cy = static_cast<int>(std::lround(center.y));
    const int r = static_cast<int>(std::ceil(radius));

    for (int dy = -r; dy <= r; ++dy) {
        for (int dx = -r; dx <= r; ++dx) {
            if (dx * dx + dy * dy <= radius * radius) {
                canvas.set_pixel(cx + dx, cy + dy, c);   // set_pixel ignores off-canvas pixels
            }
        }
    }
}

}  // namespace detail

// The double versions call the template with the types written out
// (scatter<double, double>), otherwise they would call themselves forever.
void scatter(Canvas& canvas, Axes& axes,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style) {
    scatter<double, double>(canvas, axes, x, y, style);
}

void scatter(Canvas& canvas, Rect area,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style) {
    scatter<double, double>(canvas, area, x, y, style);
}

}  // end of namespace vizlib
