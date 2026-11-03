#pragma once

#include <memory>
#include <vector>
#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/concepts.hpp"
#include "vizlib/geometry.hpp"
#include "vizlib/histogram.hpp"
#include "vizlib/kde.hpp"
#include "vizlib/kde_plot.hpp"
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
namespace detail {
// The values that are not NaN, as doubles. The KDE margins use this, because a
// single NaN would make the whole curve NaN. (Scatter and histogram skip NaN
// themselves.) In joint_plot.cpp.
std::vector<double> without_nan(std::vector<double> values);
}  // namespace detail

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
    // the margins. x and y can be any number types, even different ones (for
    // example int on x and double on y). Throws like ScatterPlot and HistogramPlot.
    template <Numeric TX, Numeric TY>
    static JointPlot with_histograms(const std::vector<TX>& x, const std::vector<TY>& y,
                                     const std::vector<double>& x_edges,
                                     const std::vector<double>& y_edges,
                                     const ScatterStyle& style = {}) {
        HistogramStyle top_style;
        top_style.color = style.color;
        HistogramStyle right_style = top_style;
        right_style.orientation = Orientation::Horizontal;

        // three different plot types, all stored as Plot pointers
        return JointPlot(std::make_unique<ScatterPlot<TX, TY>>(x, y, style),
                         std::make_unique<HistogramPlot>(x, x_edges, top_style),
                         std::make_unique<HistogramPlot>(y, y_edges, right_style));
    }

    // Same, with the bin edges from auto_bin_edges() (Member 2's binning rules).
    template <Numeric TX, Numeric TY>
    static JointPlot with_histograms(const std::vector<TX>& x, const std::vector<TY>& y,
                                     const ScatterStyle& style = {}) {
        return with_histograms(x, y, auto_bin_edges(x), auto_bin_edges(y), style);
    }

    // double versions, for brace lists like with_histograms({1, 2}, {3, 4}, ...),
    // which a template cannot work out the types from.
    static JointPlot with_histograms(const std::vector<double>& x, const std::vector<double>& y,
                                     const std::vector<double>& x_edges,
                                     const std::vector<double>& y_edges,
                                     const ScatterStyle& style = {});
    static JointPlot with_histograms(const std::vector<double>& x, const std::vector<double>& y,
                                     const ScatterStyle& style = {});

    // Scatter in the centre, KDE curves of x and y in the margins. The curves
    // come from Member 2's kde() (Gaussian kernel, Silverman's bandwidth) on
    // `grid_points` points; NaN values are left out of the curves. x and y can
    // be any number types. Throws std::invalid_argument like ScatterPlot, or if
    // x or y has no value that is not NaN.
    template <Numeric TX, Numeric TY>
    static JointPlot with_kde(const std::vector<TX>& x, const std::vector<TY>& y,
                              const ScatterStyle& style = {}, int grid_points = 200) {
        KdeStyle top_style;
        top_style.color = style.color;
        KdeStyle right_style = top_style;
        right_style.orientation = Orientation::Horizontal;

        // the scatter is built first, so a length mismatch is reported as such
        auto center = std::make_unique<ScatterPlot<TX, TY>>(x, y, style);
        const KdeResult x_curve = kde(detail::without_nan(to_doubles(x)), grid_points);
        const KdeResult y_curve = kde(detail::without_nan(to_doubles(y)), grid_points);

        return JointPlot(std::move(center),
                         std::make_unique<KdePlot>(x_curve, top_style),
                         std::make_unique<KdePlot>(y_curve, right_style));
    }

    // double version, for brace lists
    static JointPlot with_kde(const std::vector<double>& x, const std::vector<double>& y,
                              const ScatterStyle& style = {}, int grid_points = 200);

    // Works out the three panel areas inside `area` and sets their data
    // ranges. Throws std::invalid_argument if the layout leaves no room for a
    // panel (margin_ratio must be between 0 and 1, gap not negative).
    JointAxes layout(Rect area, const JointLayout& layout = {}) const;

    // Draws all three panels inside `area` of the canvas.
    void draw(Canvas& canvas, Rect area, const JointLayout& layout = {}) const;
};

}  // end of namespace vizlib
