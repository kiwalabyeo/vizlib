
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
      yScale_(0.0, 1.0, 1.0, 0.0),
      xLogScale_(1.0, 10.0, 0.0, 1.0),
      yLogScale_(1.0, 10.0, 1.0, 0.0),
      xScaleType_(ScaleType::Linear),
      yScaleType_(ScaleType::Linear) {
}

void Axes::setDataRange(double xMin, double xMax,
                        double yMin, double yMax) {
    if (!std::isfinite(xMin) || !std::isfinite(xMax) ||
        !std::isfinite(yMin) || !std::isfinite(yMax) ||
        xMin >= xMax || yMin >= yMax) {
        throw std::invalid_argument("Invalid data range");
    }

    xScale_.setRange(
        xMin, xMax,
        plotArea_.x,
        plotArea_.x + plotArea_.width
    );

    yScale_.setRange(
        yMin, yMax,
        plotArea_.y + plotArea_.height,
        plotArea_.y
    );

    if (xScaleType_ == ScaleType::Logarithmic) {
        xLogScale_.setRange(
            xMin, xMax,
            plotArea_.x,
            plotArea_.x + plotArea_.width
        );
    }

    if (yScaleType_ == ScaleType::Logarithmic) {
        yLogScale_.setRange(
            yMin, yMax,
            plotArea_.y + plotArea_.height,
            plotArea_.y
        );
    }

    xMin_ = xMin;
    xMax_ = xMax;
    yMin_ = yMin;
    yMax_ = yMax;
}

void Axes::setPlotArea(Rect area) {
    if (!std::isfinite(area.x) || !std::isfinite(area.y) ||
        !std::isfinite(area.width) ||
        !std::isfinite(area.height) ||
        !std::isfinite(area.x + area.width) ||
        !std::isfinite(area.y + area.height) ||
        area.width <= 0 || area.height <= 0) {
        throw std::invalid_argument("Invalid plot area");
    }

    xScale_.setRange(
        xMin_, xMax_,
        area.x,
        area.x + area.width
    );

    yScale_.setRange(
        yMin_, yMax_,
        area.y + area.height,
        area.y
    );

    if (xScaleType_ == ScaleType::Logarithmic) {
        xLogScale_.setRange(
            xMin_, xMax_,
            area.x,
            area.x + area.width
        );
    }

    if (yScaleType_ == ScaleType::Logarithmic) {
        yLogScale_.setRange(
            yMin_, yMax_,
            area.y + area.height,
            area.y
        );
    }

    plotArea_ = area;
}

void Axes::setXScale(ScaleType type) {
    if (type == ScaleType::Logarithmic) {
        xLogScale_.setRange(
            xMin_, xMax_,
            plotArea_.x,
            plotArea_.x + plotArea_.width
        );
    }

    xScaleType_ = type;
}

void Axes::setYScale(ScaleType type) {
    if (type == ScaleType::Logarithmic) {
        yLogScale_.setRange(
            yMin_, yMax_,
            plotArea_.y + plotArea_.height,
            plotArea_.y
        );
    }

    yScaleType_ = type;
}

Point Axes::dataToPixel(double x, double y) const {
    double pixelX;
    double pixelY;

    if (xScaleType_ == ScaleType::Logarithmic) {
        pixelX = xLogScale_.dataToPixel(x);
    } else {
        pixelX = xScale_.dataToPixel(x);
    }

    if (yScaleType_ == ScaleType::Logarithmic) {
        pixelY = yLogScale_.dataToPixel(y);
    } else {
        pixelY = yScale_.dataToPixel(y);
    }

    return Point{pixelX, pixelY};
}

Point Axes::pixelToData(double x, double y) const {
    double dataX;
    double dataY;

    if (xScaleType_ == ScaleType::Logarithmic) {
        dataX = xLogScale_.pixelToData(x);
    } else {
        dataX = xScale_.pixelToData(x);
    }

    if (yScaleType_ == ScaleType::Logarithmic) {
        dataY = yLogScale_.pixelToData(y);
    } else {
        dataY = yScale_.pixelToData(y);
    }

    return Point{dataX, dataY};
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
        xScaleType_ == ScaleType::Logarithmic
            ? generateLogTicks(xMin_, xMax_)    // 1, 10, 100, ...
            : generateTicks(xMin_, xMax_);

    const std::vector<double> yTicks =
        yScaleType_ == ScaleType::Logarithmic
            ? generateLogTicks(yMin_, yMax_)
            : generateTicks(yMin_, yMax_);

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

void Axes::drawAxes(
    Canvas& canvas,
    Color color,
    int tickLength
) const {
    if (tickLength < 0) {
        throw std::invalid_argument("Tick length cannot be negative");
    }

    const double left = plotArea_.x;
    const double right = left + plotArea_.width;
    const double top = plotArea_.y;
    const double bottom = top + plotArea_.height;

    // Draw X-axis and Y-axis.
    canvas.draw_line(Point{left, bottom}, Point{right, bottom}, color);
    canvas.draw_line(Point{left, bottom}, Point{left, top}, color);

    // Generate and draw X-axis tick marks.
    std::vector<double> xTicks =
        xScaleType_ == ScaleType::Logarithmic
        ? generateLogTicks(xMin_, xMax_)
        : generateTicks(xMin_, xMax_);

    for (double value : xTicks) {
        double x = dataToPixel(value, yMin_).x;

        canvas.draw_line(
            Point{x, bottom},
            Point{x, bottom + tickLength},
            color
        );
    }

    // Generate and draw Y-axis tick marks.
    std::vector<double> yTicks =
        yScaleType_ == ScaleType::Logarithmic
        ? generateLogTicks(yMin_, yMax_)
        : generateTicks(yMin_, yMax_);

    for (double value : yTicks) {
        double y = dataToPixel(xMin_, value).y;

        canvas.draw_line(
            Point{left - tickLength, y},
            Point{left, y},
            color
        );
    }
}

} // namespace vizlib
