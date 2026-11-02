#include "vizlib/kde_plot.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace vizlib {

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

KdePlot::KdePlot(const KdeResult& result, KdeStyle style)
    : KdePlot(result.grid, result.density, style) {}

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

    // Member 6's Canvas::draw_line takes a whole number of pixels for the thickness
    const int thickness = std::max(1, static_cast<int>(std::lround(style_.thickness)));
    for (std::size_t i = 0; i + 1 < grid_.size(); ++i) {
        canvas.draw_line(to_pixel(i), to_pixel(i + 1), style_.color, thickness);
    }
}

}  // end of namespace vizlib
