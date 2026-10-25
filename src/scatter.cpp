#include "vizlib/scatter.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

namespace vizlib {

namespace {

// Finds the range of the values that will be plotted, then adds 5% padding.
// Only points where neither x nor y is NaN are counted, because those are the
// only points that get drawn.
DataBounds data_bounds(const std::vector<double>& x, const std::vector<double>& y) {
    const double inf = std::numeric_limits<double>::infinity();
    DataBounds b{inf, -inf, inf, -inf};

    bool found_point = false;
    for (std::size_t i = 0; i < x.size(); ++i) {
        if (std::isnan(x[i]) || std::isnan(y[i])) {
            continue;
        }
        found_point = true;
        b.x_min = std::min(b.x_min, x[i]);
        b.x_max = std::max(b.x_max, x[i]);
        b.y_min = std::min(b.y_min, y[i]);
        b.y_max = std::max(b.y_max, y[i]);
    }
    if (!found_point) {
        throw std::invalid_argument("scatter: every point contains a NaN.");
    }

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

ScatterPlot::ScatterPlot(std::vector<double> x, std::vector<double> y, ScatterStyle style)
    : x_(std::move(x)), y_(std::move(y)), style_(style) {
    if (x_.size() != y_.size()) {
        throw std::invalid_argument("scatter: x and y must have the same length.");
    }
    if (x_.empty()) {
        throw std::invalid_argument("scatter: x and y must not be empty.");
    }
    bounds_ = data_bounds(x_, y_);
}

DataBounds ScatterPlot::bounds() const {
    return bounds_;
}

void ScatterPlot::draw(Canvas& canvas, const Axes& axes) const {
    for (std::size_t i = 0; i < x_.size(); ++i) {
        if (std::isnan(x_[i]) || std::isnan(y_[i])) {
            continue;   // a missing value: skip this point
        }
        draw_dot(canvas, axes.dataToPixel(x_[i], y_[i]), style_.radius, style_.color);
    }
}

void scatter(Canvas& canvas, Axes& axes,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style) {
    const ScatterPlot plot(x, y, style);
    const DataBounds b = plot.bounds();
    axes.setDataRange(b.x_min, b.x_max, b.y_min, b.y_max);
    plot.draw(canvas, axes);
}

void scatter(Canvas& canvas, Rect area,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style) {
    Axes axes;
    axes.setPlotArea(area);
    scatter(canvas, axes, x, y, style);
}

}  // end of namespace vizlib
