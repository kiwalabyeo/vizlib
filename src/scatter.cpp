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

// One filled dot, drawn with Member 6's Canvas::fill_circle.
void draw_dot(Canvas& canvas, Point center, double radius, Color c) {
    canvas.fill_circle(center, radius, c);
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
