// axes.cpp - Axes (Member 5); drawTickLabels by Member 8. See axes.hpp.

#include "vizlib/axes.hpp"
#include "vizlib/ticks.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace vizlib {


// The y scales map data 0..1 to pixels 1..0 (bottom to top), which flips the
// y axis. The log scales start at 1..10 because a log range cannot include 0;
// they are only used after setXScale / setYScale switches to them.
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

    // x: data min goes to the left edge, data max to the right edge
    xScale_.setRange(
        xMin, xMax,
        plotArea_.x,
        plotArea_.x + plotArea_.width
    );

    // y: data min goes to the BOTTOM edge (y + height) and data max to the
    // top, because pixel y grows downwards on an image
    yScale_.setRange(
        yMin, yMax,
        plotArea_.y + plotArea_.height,
        plotArea_.y
    );

    // the log scales are only updated when they are in use, because a
    // linear range may include 0 or negative values
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

    // stored last, so a range that was refused above changes nothing
    xMin_ = xMin;
    xMax_ = xMax;
    yMin_ = yMin;
    yMax_ = yMax;
}

// The same as setDataRange, but this time the pixel range changes and the
// data range stays.
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

// Switching to log sets up the log scale with the current data range. If
// that range includes 0 (like the default 0..1), LogScale::setRange throws
// and the axis stays linear.
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

// Each axis uses its own scale type, so one axis can be log and the other
// linear (as in log_axis_example).
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

        const std::string label =
            formatTickLabel(tick, xTicks, xScaleType_ == ScaleType::Logarithmic);

        const TextMetrics metrics =
            renderer.measure_text(label, fontSize);

        renderer.draw_text(
            canvas,
            label,
            Point{
                position.x - metrics.width / 2.0,
                // the point is the text's baseline, so add the font size to
                // keep the whole number below the tick marks
                plotArea_.y + plotArea_.height + labelPadding + fontSize
            },
            fontSize,
            black
        );
    }

    // Draw Y-axis tick labels.
    for (double tick : yTicks) {
        const Point position = dataToPixel(xMin_, tick);

        const std::string label =
            formatTickLabel(tick, yTicks, yScaleType_ == ScaleType::Logarithmic);

        const TextMetrics metrics =
            renderer.measure_text(label, fontSize);

        renderer.draw_text(
            canvas,
            label,
            Point{
                plotArea_.x - metrics.width - labelPadding,
                position.y + fontSize * 0.35   // centre the digits on the tick
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

    // the four edges of the plot area, in pixels
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

    // each x tick mark hangs down from the x axis line; only its pixel x
    // is needed, so any valid y (yMin_) is passed with it
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

    // each y tick mark sticks out to the left of the y axis line
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
