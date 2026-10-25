#pragma once

#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"

namespace vizlib {

// the smallest and largest x and y a plot needs, in data units
struct DataBounds {
    double x_min = 0;
    double x_max = 1;
    double y_min = 0;
    double y_max = 1;
};

// Base class for everything that draws data onto an Axes: scatter, histogram,
// KDE curve, and later ECDF. A JointPlot only knows about Plot, so any kind of
// plot can be used for any of its panels.
//
// Drawing is split into two steps so several plots can share a scale:
//   1. bounds() says what data range the plot needs.
//   2. The caller sets the Axes range (possibly wider, to match other panels).
//   3. draw() draws into that Axes without changing it.
class Plot {
public:
    virtual ~Plot() = default;

    virtual DataBounds bounds() const = 0;
    virtual void draw(Canvas& canvas, const Axes& axes) const = 0;
};

}  // end of namespace vizlib
