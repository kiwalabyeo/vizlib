#pragma once

#include <vector>
#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"
#include "vizlib/geometry.hpp"

namespace vizlib {

// how the dots of a scatter plot look
struct ScatterStyle {
    Color color{31, 119, 180, 255};   // blue, the same default colour seaborn uses
    double radius = 4.0;              // dot radius in pixels
};

// Draws one filled dot per (x[i], y[i]) pair inside `area` on the canvas.
//
// The data range is the min and max of the data plus 5% padding on each side,
// so no dot sits on the border. Larger y values are drawn higher up.
//
// Throws std::invalid_argument if x and y have different lengths, are empty,
// or contain no point without a NaN. Points with a NaN in x or y are skipped.
//
// Week 2 version: `area` stands in for an Axes until the Axes class exists.
void scatter(Canvas& canvas, Rect area,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style = {});

}  // end of namespace vizlib
