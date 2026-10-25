#pragma once

#include <vector>
#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"
#include "vizlib/geometry.hpp"
#include "vizlib/plot.hpp"

namespace vizlib {

// how the dots of a scatter plot look
struct ScatterStyle {
    Color color{31, 119, 180, 255};   // blue, the same default colour seaborn uses
    double radius = 4.0;              // dot radius in pixels
};

// A scatter plot: one filled dot per (x[i], y[i]) pair.
//
// bounds() is the min and max of the data plus 5% padding on each side, so no
// dot sits on the border. Points with a NaN in x or y are skipped.
//
// The constructor throws std::invalid_argument if x and y have different
// lengths, are empty, or contain no point without a NaN, so a ScatterPlot that
// exists can always be drawn.
class ScatterPlot : public Plot {
private:
    std::vector<double> x_;
    std::vector<double> y_;
    ScatterStyle style_;
    DataBounds bounds_;

public:
    ScatterPlot(std::vector<double> x, std::vector<double> y, ScatterStyle style = {});

    DataBounds bounds() const override;
    void draw(Canvas& canvas, const Axes& axes) const override;
};

// Draws a scatter plot into `axes`, after setting its data range to the
// ScatterPlot bounds. Throws like the ScatterPlot constructor.
void scatter(Canvas& canvas, Axes& axes,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style = {});

// Same as above, drawing inside `area` of the canvas. Kept from Week 2 so
// existing code still works.
void scatter(Canvas& canvas, Rect area,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style = {});

}  // end of namespace vizlib
