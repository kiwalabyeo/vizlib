#include "vizlib/histogram.hpp"
#include "vizlib/stats.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace vizlib {

namespace detail {

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

}  // namespace detail

namespace {

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

// The double versions call the template with the type written out, otherwise
// they would call themselves forever.
std::vector<std::size_t> histogram_counts(const std::vector<double>& data,
                                          const std::vector<double>& edges) {
    return histogram_counts<double>(data, edges);
}

std::vector<double> auto_bin_edges(const std::vector<double>& data) {
    std::vector<double> sorted;
    for (double v : data) {
        if (!std::isnan(v)) {
            sorted.push_back(v);
        }
    }
    if (sorted.empty()) {
        throw std::invalid_argument("histogram: data has no value that is not NaN.");
    }
    std::sort(sorted.begin(), sorted.end());
    const double lo = sorted.front();
    const double hi = sorted.back();
    if (lo == hi) {
        return {lo - 0.5, hi + 0.5};
    }

    // Freedman–Diaconis gives the bin width, and with it the number of bins.
    // Its last edge can overshoot the max, so the edges are spread evenly
    // from min to max instead (same as numpy).
    const summaryStatistics stats(sorted);
    std::size_t bins = stats.histogram_bin_edges().size();
    bins = bins >= 2 ? bins - 1 : 0;
    if (bins == 0) {
        // IQR is 0: Freedman–Diaconis cannot pick a width, use Sturges' rule
        bins = static_cast<std::size_t>(std::ceil(std::log2(static_cast<double>(sorted.size())))) + 1;
    }

    std::vector<double> edges;
    for (std::size_t i = 0; i <= bins; ++i) {
        edges.push_back(lo + (hi - lo) * static_cast<double>(i) / static_cast<double>(bins));
    }
    edges.back() = hi;   // no rounding error on the last edge
    return edges;
}

HistogramPlot::HistogramPlot(const std::vector<double>& data, std::vector<double> edges,
                             HistogramStyle style)
    : edges_(std::move(edges)), style_(style) {
    detail::check_histogram_data(data);
    counts_ = histogram_counts<double>(data, edges_);
}

const std::vector<std::size_t>& HistogramPlot::counts() const {
    return counts_;
}

DataBounds HistogramPlot::bounds() const {
    const std::size_t tallest = *std::max_element(counts_.begin(), counts_.end());
    // if nothing landed in a bin, still give the count axis a height of 1
    const double count_max = tallest == 0 ? 1.0 : 1.05 * static_cast<double>(tallest);

    if (style_.orientation == Orientation::Vertical) {
        return {edges_.front(), edges_.back(), 0.0, count_max};
    }
    return {0.0, count_max, edges_.front(), edges_.back()};
}

void HistogramPlot::draw(Canvas& canvas, const Axes& axes) const {
    const bool vertical = style_.orientation == Orientation::Vertical;

    for (std::size_t i = 0; i < counts_.size(); ++i) {
        if (counts_[i] == 0) {
            continue;
        }
        const double count = static_cast<double>(counts_[i]);
        // one corner of the bar sits on the base line, the other at its height
        const Point base = vertical ? axes.dataToPixel(edges_[i], 0.0)
                                    : axes.dataToPixel(0.0, edges_[i]);
        const Point top = vertical ? axes.dataToPixel(edges_[i + 1], count)
                                   : axes.dataToPixel(count, edges_[i + 1]);
        fill_box(canvas, base, top, style_.color);
    }
}

void histogram(Canvas& canvas, Axes& axes,
               const std::vector<double>& data, const std::vector<double>& edges,
               const HistogramStyle& style) {
    histogram<double>(canvas, axes, data, edges, style);
}

}  // end of namespace vizlib
