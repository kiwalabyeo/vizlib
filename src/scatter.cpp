#include "vizlib/scatter.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace vizlib {

namespace {

// the smallest and largest value of a data range
struct Range {
    double min = 0;
    double max = 0;
};

// Finds the range of the values that will be plotted, then adds 5% padding.
// Only points where neither x nor y is NaN are counted, because those are the
// only points that get drawn.
void data_ranges(const std::vector<double>& x, const std::vector<double>& y,
                 Range& x_range, Range& y_range) {
    const double inf = std::numeric_limits<double>::infinity();
    x_range = {inf, -inf};
    y_range = {inf, -inf};

    bool found_point = false;
    for (std::size_t i = 0; i < x.size(); ++i) {
        if (std::isnan(x[i]) || std::isnan(y[i])) {
            continue;
        }
        found_point = true;
        x_range.min = std::min(x_range.min, x[i]);
        x_range.max = std::max(x_range.max, x[i]);
        y_range.min = std::min(y_range.min, y[i]);
        y_range.max = std::max(y_range.max, y[i]);
    }
    if (!found_point) {
        throw std::invalid_argument("scatter: every point contains a NaN.");
    }

    for (Range* r : {&x_range, &y_range}) {
        double span = r->max - r->min;
        if (span == 0) {
            // all values are equal: give the range a width of 1 so we never
            // divide by zero, and the dots land in the middle
            r->min -= 0.5;
            r->max += 0.5;
            span = 1.0;
        }
        r->min -= 0.05 * span;
        r->max += 0.05 * span;
    }
}

// Converts a data value to a fraction from 0 (range min) to 1 (range max).
double fraction(double value, Range r) {
    return (value - r.min) / (r.max - r.min);
}

// Draws a filled circle by colouring every pixel whose distance from the
// centre is at most `radius`. Temporary: to be replaced by Canvas::fill_circle
// once Member 6's implementation is merged.
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

}  // end of unnamed namespace

void scatter(Canvas& canvas, Rect area,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("scatter: x and y must have the same length.");
    }
    if (x.empty()) {
        throw std::invalid_argument("scatter: x and y must not be empty.");
    }

    Range x_range, y_range;
    data_ranges(x, y, x_range, y_range);

    for (std::size_t i = 0; i < x.size(); ++i) {
        if (std::isnan(x[i]) || std::isnan(y[i])) {
            continue;   // a missing value: skip this point
        }
        // x grows to the right. Pixel y grows downwards, so the y fraction is
        // flipped to put large values at the top of the area.
        Point pixel;
        pixel.x = area.x + fraction(x[i], x_range) * area.width;
        pixel.y = area.y + (1.0 - fraction(y[i], y_range)) * area.height;
        draw_dot(canvas, pixel, style.radius, style.color);
    }
}

}  // end of namespace vizlib
