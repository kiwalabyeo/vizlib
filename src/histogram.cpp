#include "vizlib/histogram.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace vizlib {

namespace {

void check_edges(const std::vector<double>& edges) {
    if (edges.size() < 2) {
        throw std::invalid_argument("histogram: need at least 2 bin edges.");
    }
    for (std::size_t i = 0; i < edges.size(); ++i) {
        if (!std::isfinite(edges[i])) {
            throw std::invalid_argument("histogram: bin edges must be finite.");
        }
        if (i > 0 && edges[i] <= edges[i - 1]) {
            throw std::invalid_argument("histogram: bin edges must be strictly increasing.");
        }
    }
}

// Colours every pixel whose top-left corner lies inside the rectangle spanned
// by the two corners. Rounding both sides the same way means neighbouring bars
// never overlap and never leave a gap. Temporary: to be replaced by
// Canvas::fill_rect once Member 6's implementation is merged.
void fill_box(Canvas& canvas, Point a, Point b, Color c) {
    const int x0 = static_cast<int>(std::lround(std::min(a.x, b.x)));
    const int x1 = static_cast<int>(std::lround(std::max(a.x, b.x)));
    const int y0 = static_cast<int>(std::lround(std::min(a.y, b.y)));
    const int y1 = static_cast<int>(std::lround(std::max(a.y, b.y)));

    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            canvas.set_pixel(x, y, c);   // set_pixel ignores off-canvas pixels
        }
    }
}

}  // end of unnamed namespace

std::vector<std::size_t> histogram_counts(const std::vector<double>& data,
                                          const std::vector<double>& edges) {
    check_edges(edges);

    std::vector<std::size_t> counts(edges.size() - 1, 0);
    for (double value : data) {
        if (std::isnan(value) || value < edges.front() || value > edges.back()) {
            continue;
        }
        // upper_bound finds the first edge greater than the value, so the
        // value belongs to the bin just before it. The right-most edge has
        // nothing after it, so a value equal to it goes in the last bin.
        auto it = std::upper_bound(edges.begin(), edges.end(), value);
        std::size_t bin = static_cast<std::size_t>(it - edges.begin()) - 1;
        if (bin == counts.size()) {
            bin = counts.size() - 1;
        }
        ++counts[bin];
    }
    return counts;
}

void histogram(Canvas& canvas, Axes& axes,
               const std::vector<double>& data, const std::vector<double>& edges,
               const HistogramStyle& style) {
    if (data.empty()) {
        throw std::invalid_argument("histogram: data must not be empty.");
    }
    if (std::all_of(data.begin(), data.end(), [](double v) { return std::isnan(v); })) {
        throw std::invalid_argument("histogram: every value is NaN.");
    }

    const std::vector<std::size_t> counts = histogram_counts(data, edges);

    const std::size_t tallest = *std::max_element(counts.begin(), counts.end());
    // if nothing landed in a bin, still give the count axis a height of 1
    const double count_max = tallest == 0 ? 1.0 : 1.05 * static_cast<double>(tallest);

    const bool vertical = style.orientation == Orientation::Vertical;
    if (vertical) {
        axes.setDataRange(edges.front(), edges.back(), 0.0, count_max);
    } else {
        axes.setDataRange(0.0, count_max, edges.front(), edges.back());
    }

    for (std::size_t i = 0; i < counts.size(); ++i) {
        if (counts[i] == 0) {
            continue;
        }
        const double count = static_cast<double>(counts[i]);
        // one corner of the bar sits on the base line, the other at its height
        const Point base = vertical ? axes.dataToPixel(edges[i], 0.0)
                                    : axes.dataToPixel(0.0, edges[i]);
        const Point top = vertical ? axes.dataToPixel(edges[i + 1], count)
                                   : axes.dataToPixel(count, edges[i + 1]);
        fill_box(canvas, base, top, style.color);
    }
}

}  // end of namespace vizlib
