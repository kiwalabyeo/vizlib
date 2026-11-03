
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

void Axes::setDataRange(
    double xMin, double xMax,
    double yMin, double yMax
) {
    if (!std::isfinite(xMin) || !std::isfinite(xMax) ||
        !std::isfinite(yMin) || !std::isfinite(yMax) ||
        xMin >= xMax || yMin >= yMax) {
        throw std::invalid_argument("Invalid data range");
    }

    xMin_ = xMin;
    xMax_ = xMax;
    yMin_ = yMin;
    yMax_ = yMax;

    xScale_.setRange(xMin_, xMax_,
                     plotArea_.x,
                     plotArea_.x + plotArea_.width);

    yScale_.setRange(yMin_, yMax_,
                     plotArea_.y + plotArea_.height,
                     plotArea_.y);

    if (xScaleType_ == ScaleType::Logarithmic) {
        xLogScale_.setRange(xMin_, xMax_,
                            plotArea_.x,
                            plotArea_.x + plotArea_.width);
    }

    if (yScaleType_ == ScaleType::Logarithmic) {
        yLogScale_.setRange(yMin_, yMax_,
                            plotArea_.y + plotArea_.height,
                            plotArea_.y);
    }
}

void Axes::setPlotArea(Rect area) {
    if (!std::isfinite(area.x) ||
        !std::isfinite(area.y) ||
        !std::isfinite(area.width) ||
        !std::isfinite(area.height) ||
        area.width <= 0.0 ||
        area.height <= 0.0) {
        throw std::invalid_argument("Invalid plot area");
    }

    plotArea_ = area;

    xScale_.setRange(xMin_, xMax_,
                     plotArea_.x,
                     plotArea_.x + plotArea_.width);

    yScale_.setRange(yMin_, yMax_,
                     plotArea_.y + plotArea_.height,
                     plotArea_.y);

    if (xScaleType_ == ScaleType::Logarithmic) {
        xLogScale_.setRange(xMin_, xMax_,
                            plotArea_.x,
                            plotArea_.x + plotArea_.width);
    }

    if (yScaleType_ == ScaleType::Logarithmic) {
        yLogScale_.setRange(yMin_, yMax_,
                            plotArea_.y + plotArea_.height,
                            plotArea_.y);
    }
}

void Axes::setXScale(ScaleType type) {
    if (type == ScaleType::Logarithmic) {
        xLogScale_.setRange(xMin_, xMax_,
                            plotArea_.x,
                            plotArea_.x + plotArea_.width);
    }

    xScaleType_ = type;
}

void Axes::setYScale(ScaleType type) {
    if (type == ScaleType::Logarithmic) {
        yLogScale_.setRange(yMin_, yMax_,
                            plotArea_.y + plotArea_.height,
                            plotArea_.y);
    }

    yScaleType_ = type;
}

Point Axes::dataToPixel(double x, double y) const {
    double pixelX = xScaleType_ == ScaleType::Logarithmic
        ? xLogScale_.dataToPixel(x)
        : xScale_.dataToPixel(x);

    double pixelY = yScaleType_ == ScaleType::Logarithmic
        ? yLogScale_.dataToPixel(y)
        : yScale_.dataToPixel(y);

    return Point{pixelX, pixelY};
}

Point Axes::pixelToData(double x, double y) const {
    double dataX = xScaleType_ == ScaleType::Logarithmic
        ? xLogScale_.pixelToData(x)
        : xScale_.pixelToData(x);

    double dataY = yScaleType_ == ScaleType::Logarithmic
        ? yLogScale_.pixelToData(y)
        : yScale_.pixelToData(y);

    return Point{dataX, dataY};
}

Rect Axes::getPlotArea() const {
    return plotArea_;
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

void Axes::drawTickLabels(
    Canvas& canvas,
    const TextRenderer& renderer
) const {
    const double fontSize = 12.0;
    const Color black{0, 0, 0};

    std::vector<double> xTicks =
        xScaleType_ == ScaleType::Logarithmic
        ? generateLogTicks(xMin_, xMax_)
        : generateTicks(xMin_, xMax_);

    std::vector<double> yTicks =
        yScaleType_ == ScaleType::Logarithmic
        ? generateLogTicks(yMin_, yMax_)
        : generateTicks(yMin_, yMax_);

    // Draw X-axis labels.
    for (double value : xTicks) {
        Point position = dataToPixel(value, yMin_);

        std::ostringstream stream;
        stream << std::setprecision(4) << value;

        std::string label = stream.str();

        TextMetrics metrics = renderer.measure_text(label, fontSize);

        renderer.draw_text(
            canvas,
            label,
            Point{
                position.x - metrics.width / 2.0,
                position.y + 8.0
            },
            fontSize,
            black
        );
    }

    // Draw Y-axis labels.
    for (double value : yTicks) {
        Point position = dataToPixel(xMin_, value);

        std::ostringstream stream;
        stream << std::setprecision(4) << value;

        std::string label = stream.str();

        TextMetrics metrics = renderer.measure_text(label, fontSize);

        renderer.draw_text(
            canvas,
            label,
            Point{
                position.x - metrics.width - 8.0,
                position.y - metrics.height / 2.0
            },
            fontSize,
            black
        );
    }
}

}
