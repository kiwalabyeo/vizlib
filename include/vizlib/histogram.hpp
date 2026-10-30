#pragma once

#include <cstddef>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>
#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"
#include "vizlib/concepts.hpp"
#include "vizlib/plot.hpp"

namespace vizlib {

// which way the bars (or a KDE curve) point
enum class Orientation {
    Vertical,     // bins along x, bars grow upwards
    Horizontal    // bins along y, bars grow to the right (used for joint plot margins)
};

// how the bars of a histogram look
struct HistogramStyle {
    Color color{31, 119, 180, 255};   // blue, the same default colour as scatter
    Orientation orientation = Orientation::Vertical;
};

namespace detail {
// Throws std::invalid_argument if there are fewer than 2 edges, or the edges
// are not finite and strictly increasing. In histogram.cpp.
void check_edges(const std::vector<double>& edges);

// Throws std::invalid_argument if `data` is empty or every value is NaN.
template <Numeric T>
void check_histogram_data(const std::vector<T>& data) {
    if (data.empty()) {
        throw std::invalid_argument("histogram: data must not be empty.");
    }
    if (std::all_of(data.begin(), data.end(),
                    [](T v) { return std::isnan(static_cast<double>(v)); })) {
        throw std::invalid_argument("histogram: every value is NaN.");
    }
}
}  // namespace detail

// Counts how many values fall in each bin. Bin i covers [edges[i], edges[i+1]),
// except the last bin, which also includes its right edge (same as numpy).
// Values outside the edges and NaN values are not counted.
//
// The data can be any number type. The edges are always double, because bin
// edges usually fall between whole numbers even when the data is int.
//
// Throws std::invalid_argument if there are fewer than 2 edges, or the edges
// are not finite and strictly increasing.
template <Numeric T>
std::vector<std::size_t> histogram_counts(const std::vector<T>& data,
                                          const std::vector<double>& edges) {
    detail::check_edges(edges);

    std::vector<std::size_t> counts(edges.size() - 1, 0);
    for (T raw : data) {
        const double value = static_cast<double>(raw);
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

// double version, for brace lists like histogram_counts({1, 2, 3}, edges),
// which a template cannot work out T from.
std::vector<std::size_t> histogram_counts(const std::vector<double>& data,
                                          const std::vector<double>& edges);

// Bin edges for `data`, evenly spaced from its min to its max. The number of
// bins comes from the Freedman–Diaconis rule (Member 2's
// summaryStatistics::histogram_bin_edges). If the IQR is 0, Sturges' rule
// (log2(n) + 1 bins) is used instead, and if every value is the same there is
// one bin of width 1 around it. NaN values are ignored.
//
// Throws std::invalid_argument if `data` has no value that is not NaN.
std::vector<double> auto_bin_edges(const std::vector<double>& data);

// Any other number type: the statistics are worked out in double anyway, so the
// data is converted once and passed to the double version above.
template <Numeric T>
std::vector<double> auto_bin_edges(const std::vector<T>& data) {
    return auto_bin_edges(to_doubles(data));
}

// A histogram: one bar per bin.
//
// The class itself is not a template. It only keeps the bin edges and the
// counts, and those do not depend on the type of the data. So only the
// constructor is a template: it takes data of any number type, counts it, and
// throws the data away. (Compare ScatterPlot, which keeps its data, so the
// whole class is a template.)
//
// bounds(): the bin axis runs from the first to the last edge, and the count
// axis from 0 to the tallest bar plus 5% padding. In Horizontal orientation
// the bins are on y and the counts on x.
//
// The constructor counts the values once and throws std::invalid_argument if
// `data` is empty or only NaN, or the edges are invalid (see histogram_counts).
class HistogramPlot : public Plot {
private:
    std::vector<double> edges_;
    std::vector<std::size_t> counts_;
    HistogramStyle style_;

public:
    template <Numeric T>
    HistogramPlot(const std::vector<T>& data, std::vector<double> edges,
                  HistogramStyle style = {})
        : edges_(std::move(edges)), style_(style) {
        detail::check_histogram_data(data);
        counts_ = histogram_counts(data, edges_);
    }

    // double version, for brace lists (see histogram_counts)
    HistogramPlot(const std::vector<double>& data, std::vector<double> edges,
                  HistogramStyle style = {});

    const std::vector<std::size_t>& counts() const;

    DataBounds bounds() const override;
    void draw(Canvas& canvas, const Axes& axes) const override;
};

// Draws a histogram into `axes`, after setting its data range to the
// HistogramPlot bounds. Throws like the HistogramPlot constructor.
//
// Pass auto_bin_edges(data) as `edges` for Freedman–Diaconis bins.
template <Numeric T>
void histogram(Canvas& canvas, Axes& axes,
               const std::vector<T>& data, const std::vector<double>& edges,
               const HistogramStyle& style = {}) {
    const HistogramPlot plot(data, edges, style);
    const DataBounds b = plot.bounds();
    axes.setDataRange(b.x_min, b.x_max, b.y_min, b.y_max);
    plot.draw(canvas, axes);
}

// double version, for brace lists
void histogram(Canvas& canvas, Axes& axes,
               const std::vector<double>& data, const std::vector<double>& edges,
               const HistogramStyle& style = {});

}  // end of namespace vizlib
