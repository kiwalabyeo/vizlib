#pragma once

#include <memory>
#include <vector>
#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/geometry.hpp"
#include "vizlib/histogram.hpp"
#include "vizlib/plot.hpp"
#include "vizlib/scatter.hpp"

namespace vizlib {

// how the three panels of a joint plot are laid out
struct JointLayout {
    double margin_ratio = 0.2;   // share of the width (and height) given to the margin panels
    double gap = 6.0;            // pixels between the centre panel and each margin
};

// the three Axes of a joint plot, with their plot areas and data ranges set
struct JointAxes {
    Axes center;
    Axes top;     // shares its x scale with center
    Axes right;   // shares its y scale with center
};

// A joint plot: a centre panel (usually a scatter plot) with a margin panel on
// top showing the distribution of x, and one on the right showing y.
//
//   +-----------+---+
//   |    top    |   |
//   +-----------+---+
//   |           | r |
//   |  center   | i |
//   |           | g |
//   +-----------+---+
//
// The panels can be any Plot. The top margin should be Vertical and the right
// margin Horizontal, so their bins line up with the centre panel.
//
// Shared scales: the x range of center and top is the union of both plots'
// x bounds, so a given x value lands in the same pixel column in both. The
// same goes for y in center and right.
class JointPlot {
private:
    std::unique_ptr<Plot> center_;
    std::unique_ptr<Plot> top_;
    std::unique_ptr<Plot> right_;

public:
    // Throws std::invalid_argument if any panel is null.
    JointPlot(std::unique_ptr<Plot> center, std::unique_ptr<Plot> top,
              std::unique_ptr<Plot> right);

    // The usual joint plot: scatter in the centre, histograms of x and y in
    // the margins. Throws like ScatterPlot and HistogramPlot.
    static JointPlot with_histograms(const std::vector<double>& x, const std::vector<double>& y,
                                     const std::vector<double>& x_edges,
                                     const std::vector<double>& y_edges,
                                     const ScatterStyle& style = {});

    // Same, with the bin edges from auto_bin_edges() (Freedman–Diaconis).
    static JointPlot with_histograms(const std::vector<double>& x, const std::vector<double>& y,
                                     const ScatterStyle& style = {});

    // Works out the three panel areas inside `area` and sets their data
    // ranges. Throws std::invalid_argument if the layout leaves no room for a
    // panel (margin_ratio must be between 0 and 1, gap not negative).
    JointAxes layout(Rect area, const JointLayout& layout = {}) const;

    // Draws all three panels inside `area` of the canvas.
    void draw(Canvas& canvas, Rect area, const JointLayout& layout = {}) const;
};

}  // end of namespace vizlib
