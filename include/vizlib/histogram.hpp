#pragma once

#include <cstddef>
#include <vector>
#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"
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

// Counts how many values fall in each bin. Bin i covers [edges[i], edges[i+1]),
// except the last bin, which also includes its right edge (same as numpy).
// Values outside the edges and NaN values are not counted.
//
// Throws std::invalid_argument if there are fewer than 2 edges, or the edges
// are not finite and strictly increasing.
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

// A histogram: one bar per bin.
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
void histogram(Canvas& canvas, Axes& axes,
               const std::vector<double>& data, const std::vector<double>& edges,
               const HistogramStyle& style = {});

}  // end of namespace vizlib
