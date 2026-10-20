#pragma once

#include <cstddef>
#include <vector>
#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"

namespace vizlib {

// which way the bars point
enum class Orientation {
    Vertical,     // bins along x, bars grow upwards
    Horizontal    // bins along y, bars grow to the right (used for joint plot margins)
};

// how the bars of a histogram look
struct HistogramStyle {
    Color color{31, 119, 180, 255};   // blue, the same default colour as scatter
    Orientation orientation = Orientation::Vertical;
};

// Counts how many values fall in each bin. Bin i covers [edges[i], edges[i+1]),
// except the last bin, which also includes its right edge (same as numpy).
// Values outside the edges and NaN values are not counted.
//
// Throws std::invalid_argument if there are fewer than 2 edges, or the edges
// are not finite and strictly increasing.
std::vector<std::size_t> histogram_counts(const std::vector<double>& data,
                                          const std::vector<double>& edges);

// Draws one bar per bin into the plot area of `axes`.
//
// Sets the data range of `axes`: the bin axis runs from the first to the last
// edge, and the count axis from 0 to the tallest bar plus 5% padding.
//
// Throws std::invalid_argument if `data` is empty or only NaN, or the edges are
// invalid (see histogram_counts).
//
// Week 3 version: the caller passes the bin edges. Once the Freedman–Diaconis
// edges in stats are fixed, an overload without `edges` will use them.
void histogram(Canvas& canvas, Axes& axes,
               const std::vector<double>& data, const std::vector<double>& edges,
               const HistogramStyle& style = {});

}  // end of namespace vizlib
