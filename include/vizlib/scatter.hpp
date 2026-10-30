#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"
#include "vizlib/concepts.hpp"
#include "vizlib/geometry.hpp"
#include "vizlib/plot.hpp"

namespace vizlib {

// how the dots of a scatter plot look
struct ScatterStyle {
    Color color{31, 119, 180, 255};   // blue, the same default colour seaborn uses
    double radius = 4.0;              // dot radius in pixels
};

namespace detail {
// Helpers shared by every ScatterPlot<TX, TY>. They work on doubles, so they are
// compiled once in scatter.cpp instead of once per type.
DataBounds pad_bounds(DataBounds b);                                   // adds the 5% padding
void draw_dot(Canvas& canvas, Point center, double radius, Color c);  // one filled dot
}  // namespace detail

// A scatter plot: one filled dot per (x[i], y[i]) pair.
//
// It is a class template, so x and y can be any number type, and each can have
// its own type: for example years as int on x and prices as double on y. The
// data is stored in its own type and only turned into double when a point is
// placed on the canvas. ScatterPlot on its own means ScatterPlot<double, double>.
//
// It still inherits from Plot, so a JointPlot can hold a ScatterPlot<int, float>
// through a Plot pointer like any other plot. The template picks the types at
// compile time, and the virtual draw() picks the plot kind at run time.
//
// bounds() is the min and max of the data plus 5% padding on each side, so no
// dot sits on the border. Points with a NaN in x or y are skipped (integers are
// never NaN).
//
// The constructor throws std::invalid_argument if x and y have different
// lengths, are empty, or contain no point without a NaN, so a ScatterPlot that
// exists can always be drawn.
template <Numeric TX = double, Numeric TY = TX>
class ScatterPlot : public Plot {
private:
    std::vector<TX> x_;
    std::vector<TY> y_;
    ScatterStyle style_;
    DataBounds bounds_;

public:
    ScatterPlot(std::vector<TX> x, std::vector<TY> y, ScatterStyle style = {})
        : x_(std::move(x)), y_(std::move(y)), style_(style) {
        if (x_.size() != y_.size()) {
            throw std::invalid_argument("scatter: x and y must have the same length.");
        }
        if (x_.empty()) {
            throw std::invalid_argument("scatter: x and y must not be empty.");
        }

        // range of the points that will be drawn (no NaN in x or y)
        const double inf = std::numeric_limits<double>::infinity();
        DataBounds b{inf, -inf, inf, -inf};
        bool found_point = false;
        for (std::size_t i = 0; i < x_.size(); ++i) {
            const double xi = static_cast<double>(x_[i]);
            const double yi = static_cast<double>(y_[i]);
            if (std::isnan(xi) || std::isnan(yi)) {
                continue;
            }
            found_point = true;
            b.x_min = std::min(b.x_min, xi);
            b.x_max = std::max(b.x_max, xi);
            b.y_min = std::min(b.y_min, yi);
            b.y_max = std::max(b.y_max, yi);
        }
        if (!found_point) {
            throw std::invalid_argument("scatter: every point contains a NaN.");
        }
        bounds_ = detail::pad_bounds(b);
    }

    DataBounds bounds() const override { return bounds_; }

    void draw(Canvas& canvas, const Axes& axes) const override {
        for (std::size_t i = 0; i < x_.size(); ++i) {
            const double xi = static_cast<double>(x_[i]);
            const double yi = static_cast<double>(y_[i]);
            if (std::isnan(xi) || std::isnan(yi)) {
                continue;   // a missing value: skip this point
            }
            detail::draw_dot(canvas, axes.dataToPixel(xi, yi), style_.radius, style_.color);
        }
    }
};

// Sets the axes' data range to fit the points, then draws them.
// Throws std::invalid_argument for the same cases as ScatterPlot.
template <Numeric TX, Numeric TY>
void scatter(Canvas& canvas, Axes& axes,
             const std::vector<TX>& x, const std::vector<TY>& y,
             const ScatterStyle& style = {}) {
    const ScatterPlot<TX, TY> plot(x, y, style);
    const DataBounds b = plot.bounds();
    axes.setDataRange(b.x_min, b.x_max, b.y_min, b.y_max);
    plot.draw(canvas, axes);
}

// Same, but builds the Axes for you inside `area` (in pixels).
template <Numeric TX, Numeric TY>
void scatter(Canvas& canvas, Rect area,
             const std::vector<TX>& x, const std::vector<TY>& y,
             const ScatterStyle& style = {}) {
    Axes axes;
    axes.setPlotArea(area);
    scatter(canvas, axes, x, y, style);
}

// double versions. A template cannot work out T from a brace list such as
// scatter(canvas, area, {1, 2, 3}, {4, 5, 6}), so these plain functions take
// that case. For a std::vector<double> the compiler also picks these, because a
// normal function wins over a template when both match exactly.
void scatter(Canvas& canvas, Axes& axes,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style = {});
void scatter(Canvas& canvas, Rect area,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style = {});

}  // end of namespace vizlib
