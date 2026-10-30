
#include "vizlib/axes.hpp"
#include "vizlib/ticks.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace vizlib {

Axes::Axes()
    : xMin_(0.0),
      xMax_(1.0),
      yMin_(0.0),
      yMax_(1.0),
      plotArea_{0.0, 0.0, 1.0, 1.0},
      xScale_(0.0, 1.0, 0.0, 1.0),
      yScale_(0.0, 1.0, 1.0, 0.0) {
}

void Axes::setDataRange(double xMin, double xMax,
                        double yMin, double yMax) {
    if (!std::isfinite(xMin) || !std::isfinite(xMax) ||
        !std::isfinite(yMin) || !std::isfinite(yMax) ||
        xMin >= xMax || yMin >= yMax) {
        throw std::invalid_argument("Invalid data range");
    }

    xMin_ = xMin;
    xMax_ = xMax;
    yMin_ = yMin;
    yMax_ = yMax;

    setPlotArea(plotArea_);
}

void Axes::setPlotArea(Rect area) {
    if (!std::isfinite(area.x) || !std::isfinite(area.y) ||
        !std::isfinite(area.width) ||
        !std::isfinite(area.height) ||
        area.width <= 0 || area.height <= 0) {
        throw std::invalid_argument("Invalid plot area");
    }

    plotArea_ = area;

    xScale_.setRange(
        xMin_, xMax_,
        plotArea_.x,
        plotArea_.x + plotArea_.width
    );

    yScale_.setRange(
        yMin_, yMax_,
        plotArea_.y + plotArea_.height,
        plotArea_.y
    );
}

Point Axes::dataToPixel(double x, double y) const {
    return Point{
        xScale_.dataToPixel(x),
        yScale_.dataToPixel(y)
    };
}

Point Axes::pixelToData(double x, double y) const {
    return Point{
        xScale_.pixelToData(x),
        yScale_.pixelToData(y)
    };
}

Rect Axes::getPlotArea() const {
    return plotArea_;
}

void Axes::drawTickLabels(
    Canvas& canvas,
    const TextRenderer& renderer
) const {
    constexpr double fontSize = 11.0;
    constexpr double labelPadding = 8.0;

    const Color black{0, 0, 0};

    const std::vector<double> xTicks =
        generateTicks(xMin_, xMax_);

    const std::vector<double> yTicks =
        generateTicks(yMin_, yMax_);

    // Draw X-axis tick labels.
    for (double tick : xTicks) {
        const Point position = dataToPixel(tick, yMin_);

        std::ostringstream label;
        label << std::setprecision(4) << tick;

        const TextMetrics metrics =
            renderer.measure_text(label.str(), fontSize);

        renderer.draw_text(
            canvas,
            label.str(),
            Point{
                position.x - metrics.width / 2.0,
                plotArea_.y + plotArea_.height + labelPadding
            },
            fontSize,
            black
        );
    }

    // Draw Y-axis tick labels.
    for (double tick : yTicks) {
        const Point position = dataToPixel(xMin_, tick);

        std::ostringstream label;
        label << std::setprecision(4) << tick;

        const TextMetrics metrics =
            renderer.measure_text(label.str(), fontSize);

        renderer.draw_text(
            canvas,
            label.str(),
            Point{
                plotArea_.x - metrics.width - labelPadding,
                position.y
            },
            fontSize,
            black
        );
    }
}

} // namespace vizlib
