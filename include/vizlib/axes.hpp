#pragma once

#include "vizlib/geometry.hpp"
#include "vizlib/scale.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

#include <vector>

namespace vizlib {

enum class ScaleType {
    Linear,
    Logarithmic
};

class Axes {
private:
    double xMin_;
    double xMax_;
    double yMin_;
    double yMax_;

    Rect plotArea_;

    LinearScale xScale_;
    LinearScale yScale_;

    LogScale xLogScale_;
    LogScale yLogScale_;

    ScaleType xScaleType_;
    ScaleType yScaleType_;

public:
    Axes();

    void setDataRange(double xMin, double xMax,
                      double yMin, double yMax);

    void setPlotArea(Rect area);

    void setXScale(ScaleType type);
    void setYScale(ScaleType type);

    Point dataToPixel(double x, double y) const;

    Point pixelToData(double x, double y) const;

    Rect getPlotArea() const;

    void drawTickLabels(
        Canvas& canvas,
        const TextRenderer& renderer
    ) const;

    // The tick values of each axis: "nice" values from generateTicks() on a
    // linear axis, or powers of 10 from generateLogTicks() on a log axis.
    // drawTickLabels() and drawAxisLines() both use these, so every number
    // sits under its own tick mark.
    std::vector<double> xTicks() const;
    std::vector<double> yTicks() const;

    // Draws the x axis along the bottom of the plot area and the y axis along
    // its left side, with a tick mark of `tickLength` pixels sticking out at
    // every tick. Throws std::invalid_argument if tickLength is negative.
    void drawAxisLines(
        Canvas& canvas,
        Color color = Color{60, 60, 60, 255},
        int tickLength = 5
    ) const;
};

}
