#pragma once

#include "vizlib/geometry.hpp"
#include "vizlib/scale.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

// axes.hpp - Axes (Member 5)
//
// An Axes is one plot area on the image: a pixel rectangle (setPlotArea)
// plus the data range shown in it (setDataRange). Every plot draws through
// dataToPixel, so the plots never need to know about pixels or scales.
//
// Each axis keeps both a LinearScale and a LogScale (composition: an Axes HAS
// scales), and a ScaleType says which one is used. drawAxes draws the axis
// lines and tick marks; drawTickLabels (Member 8) writes the numbers.

namespace vizlib {

// enum class, not a bool or a plain enum: the names are scoped
// (ScaleType::Logarithmic), they never turn into an int by accident, and
// setXScale(ScaleType::Logarithmic) reads better than setXScale(true).
enum class ScaleType {
    Linear,
    Logarithmic
};

class Axes {
private:
    // the data range shown (default 0 to 1 on both axes)
    double xMin_;
    double xMax_;
    double yMin_;
    double yMax_;

    // where on the image the plot is drawn, in pixels
    Rect plotArea_;

    // both kinds of scale for each axis; the ScaleType picks which is used
    LinearScale xScale_;
    LinearScale yScale_;

    LogScale xLogScale_;
    LogScale yLogScale_;

    ScaleType xScaleType_;
    ScaleType yScaleType_;

public:
    // data 0 to 1 on both axes, a 1 x 1 pixel area, linear scales
    Axes();

    // Sets the data range shown, and updates the scales. Throws
    // std::invalid_argument for NaN, infinity or min >= max. On a log axis
    // the minimum must also be above 0.
    void setDataRange(double xMin, double xMax,
                      double yMin, double yMax);

    // Sets the pixel rectangle the plot is drawn in. Throws for a rectangle
    // with no width or height, or with NaN or infinite values.
    void setPlotArea(Rect area);

    // Switches an axis between linear and log. Order matters: call
    // setDataRange first, because a log axis needs a range above 0, and the
    // default range 0..1 includes 0 (setXScale would throw).
    void setXScale(ScaleType type);
    void setYScale(ScaleType type);

    // Data value -> pixel position, using the chosen scale for each axis.
    // The y axis is flipped: a bigger y gives a smaller pixel y (higher up).
    Point dataToPixel(double x, double y) const;

    // Pixel position -> data value (the inverse of dataToPixel)
    Point pixelToData(double x, double y) const;

    Rect getPlotArea() const;

    // Draws the x axis line along the bottom of the plot area, the y axis
    // line along the left, and a tick mark at every tick (nice ticks on a
    // linear axis, powers of 10 on a log axis). Throws if tickLength < 0.
    void drawAxes(
        Canvas& canvas,
        Color color,
        int tickLength = 5
    ) const;

    // Writes the number under each x tick and left of each y tick (Member 8).
    void drawTickLabels(
        Canvas& canvas,
        const TextRenderer& renderer
    ) const;
};

}
