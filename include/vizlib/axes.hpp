#pragma once

#include "vizlib/geometry.hpp"
#include "vizlib/scale.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

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

    void drawAxes(
        Canvas& canvas,
        Color color,
        int tickLength = 5
    ) const;

    void drawTickLabels(
        Canvas& canvas,
        const TextRenderer& renderer
    ) const;
};

}
