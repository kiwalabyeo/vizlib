#include "vizlib/kde_plot.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace vizlib {

namespace {

// Draws a thick straight line by stamping a square of `thickness` pixels at
// every pixel step from `a` to `b`. Temporary: to be replaced by
// Canvas::draw_line once Member 6's implementation is merged.
void draw_segment(Canvas& canvas, Point a, Point b, double thickness, Color c) {
    const double dx = b.x - a.x;
    const double dy = b.y - a.y;
    const int steps = std::max(1, static_cast<int>(std::ceil(std::max(std::abs(dx), std::abs(dy)))));
    const int half = static_cast<int>(std::floor(thickness / 2));
    const int extra = static_cast<int>(std::lround(thickness)) % 2 == 0 ? 1 : 0;   // even widths are one pixel lopsided

    for (int s = 0; s <= steps; ++s) {
        const double t = static_cast<double>(s) / steps;
        const int x = static_cast<int>(std::lround(a.x + t * dx));
        const int y = static_cast<int>(std::lround(a.y + t * dy));
        for (int oy = -half + extra; oy <= half; ++oy) {
            for (int ox = -half + extra; ox <= half; ++ox) {
                canvas.set_pixel(x + ox, y + oy, c);   // set_pixel ignores off-canvas pixels
            }
        }
    }
}

}  // end of unnamed namespace

KdePlot::KdePlot(std::vector<double> grid, std::vector<double> density, KdeStyle style)
    : grid_(std::move(grid)), density_(std::move(density)), style_(style) {
    if (grid_.size() != density_.size()) {
        throw std::invalid_argument("kde: grid and density must have the same length.");
    }
    if (grid_.size() < 2) {
        throw std::invalid_argument("kde: need at least 2 points.");
    }
    for (std::size_t i = 0; i < grid_.size(); ++i) {
        if (!std::isfinite(grid_[i]) || (i > 0 && grid_[i] <= grid_[i - 1])) {
            throw std::invalid_argument("kde: grid must be finite and strictly increasing.");
        }
        if (!std::isfinite(density_[i]) || density_[i] < 0) {
            throw std::invalid_argument("kde: density must be finite and not negative.");
        }
    }
    if (style_.thickness <= 0) {
        throw std::invalid_argument("kde: thickness must be positive.");
    }
}

DataBounds KdePlot::bounds() const {
    const double highest = *std::max_element(density_.begin(), density_.end());
    // a curve that is 0 everywhere still gets a density axis of height 1
    const double density_max = highest == 0 ? 1.0 : 1.05 * highest;

    if (style_.orientation == Orientation::Vertical) {
        return {grid_.front(), grid_.back(), 0.0, density_max};
    }
    return {0.0, density_max, grid_.front(), grid_.back()};
}

void KdePlot::draw(Canvas& canvas, const Axes& axes) const {
    const bool vertical = style_.orientation == Orientation::Vertical;
    auto to_pixel = [&](std::size_t i) {
        return vertical ? axes.dataToPixel(grid_[i], density_[i])
                        : axes.dataToPixel(density_[i], grid_[i]);
    };

    for (std::size_t i = 0; i + 1 < grid_.size(); ++i) {
        draw_segment(canvas, to_pixel(i), to_pixel(i + 1), style_.thickness, style_.color);
    }
}

}  // end of namespace vizlib
