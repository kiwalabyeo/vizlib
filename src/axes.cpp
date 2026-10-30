
#pragma once

#include "vizlib/geometry.hpp"
#include "vizlib/scale.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

namespace vizlib {

class Axes {
private:
    double xMin_;
    double xMax_;
    double yMin_;
    double yMax_;

    Rect plotArea_;

    LinearScale xScale_;
    LinearScale yScale_;

public:
    Axes();

    void setDataRange(double xMin, double xMax,
                      double yMin, double yMax);

    void setPlotArea(Rect area);

    Point dataToPixel(double x, double y) const;

    Point pixelToData(double x, double y) const;

    Rect getPlotArea() const;

    void drawTickLabels(
        Canvas& canvas,
        const TextRenderer& renderer
    ) const;
};

}
