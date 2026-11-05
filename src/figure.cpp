// figure.cpp - Figure (Member 5); title and labels by Member 8. See figure.hpp.

#include "vizlib/figure.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>

namespace vizlib {

Figure::Figure(int width, int height)
    : width_(width),
      height_(height),
      leftMargin_(60),
      topMargin_(30),
      rightMargin_(30),
      bottomMargin_(50) {
    setSize(width, height);
}

void Figure::setSize(int width, int height) {
    if (width <= 0 || height <= 0 ||
        leftMargin_ + rightMargin_ >= width ||
        topMargin_ + bottomMargin_ >= height) {
        throw std::invalid_argument("Invalid figure size");
    }

    width_ = width;
    height_ = height;
}

void Figure::setMargins(double left, double top,
                        double right, double bottom) {
    if (!std::isfinite(left) || !std::isfinite(top) ||
        !std::isfinite(right) || !std::isfinite(bottom) ||
        left < 0 || top < 0 || right < 0 || bottom < 0 ||
        left + right >= width_ ||
        top + bottom >= height_) {
        throw std::invalid_argument("Invalid figure margins");
    }

    leftMargin_ = left;
    topMargin_ = top;
    rightMargin_ = right;
    bottomMargin_ = bottom;
}

Axes& Figure::addAxes() {
    return addAxes(Rect{
        0.0, 0.0,
        static_cast<double>(width_),
        static_cast<double>(height_)
    });
}

// The plot area is the given area minus the margins on each side.
// Example: Figure(800, 600).addAxes() gives the plot area x 60, y 30,
// 710 wide (800 - 60 - 30) and 520 high (600 - 30 - 50).
Axes& Figure::addAxes(Rect area) {
    if (!std::isfinite(area.x) || !std::isfinite(area.y) ||
        !std::isfinite(area.width) ||
        !std::isfinite(area.height) ||
        area.width <= leftMargin_ + rightMargin_ ||
        area.height <= topMargin_ + bottomMargin_) {
        throw std::invalid_argument("Invalid axes area");
    }

    Rect plotArea{
        area.x + leftMargin_,
        area.y + topMargin_,
        area.width - leftMargin_ - rightMargin_,
        area.height - topMargin_ - bottomMargin_
    };

    Axes newAxes;
    newAxes.setPlotArea(plotArea);

    axes_.push_back(newAxes);

    return axes_.back();
}

Axes& Figure::getAxes(std::size_t index) {
    if (index >= axes_.size()) {
        throw std::out_of_range("Axes index is out of range");
    }

    return axes_[index];
}

const Axes& Figure::getAxes(std::size_t index) const {
    if (index >= axes_.size()) {
        throw std::out_of_range("Axes index is out of range");
    }

    return axes_[index];
}

std::size_t Figure::getAxesCount() const {
    return axes_.size();
}

int Figure::getWidth() const {
    return width_;
}

int Figure::getHeight() const {
    return height_;
}

void Figure::setTitle(const std::string& title) {
    title_ = title;
}

const std::string& Figure::getTitle() const {
    return title_;
}

void Figure::setXLabel(const std::string& label) {
    xLabel_ = label;
}

const std::string& Figure::getXLabel() const {
    return xLabel_;
}

void Figure::setYLabel(const std::string& label) {
    yLabel_ = label;
}

const std::string& Figure::getYLabel() const {
    return yLabel_;
}

void Figure::drawLabels(
    Canvas& canvas,
    const TextRenderer& renderer
) const {
    const Color black{0, 0, 0};

    Rect plotArea{
        leftMargin_,
        topMargin_,
        width_ - leftMargin_ - rightMargin_,
        height_ - topMargin_ - bottomMargin_
    };

    if (!axes_.empty()) {
        plotArea = axes_.front().getPlotArea();
    }

    // Draw the figure title centred over the plot area.
    if (!title_.empty()) {
        constexpr double titleFontSize = 18.0;

        const TextMetrics metrics =
            renderer.measure_text(title_, titleFontSize);

        const double x =
            plotArea.x + (plotArea.width - metrics.width) / 2.0;

        const double baseline = topMargin_ - 5.0;

        renderer.draw_text(
            canvas,
            title_,
            Point{x, baseline},
            titleFontSize,
            black
        );
    }

    // Draw the X-axis label closer to the plot.
    if (!xLabel_.empty()) {
        constexpr double labelFontSize = 12.0;

        const TextMetrics metrics =
            renderer.measure_text(xLabel_, labelFontSize);

        const double x =
            plotArea.x + (plotArea.width - metrics.width) / 2.0;

        const double baseline =
            plotArea.y + plotArea.height + 40.0;   // below the tick marks and tick numbers

        renderer.draw_text(
            canvas,
            xLabel_,
            Point{x, baseline},
            labelFontSize,
            black
        );
    }

    // Keep the Y-axis label inside the canvas.
    if (!yLabel_.empty()) {
        constexpr double labelFontSize = 12.0;

        const TextMetrics metrics =
            renderer.measure_text(yLabel_, labelFontSize);

        const double x =
            std::max(5.0, plotArea.x - metrics.width);

        const double baseline =
            plotArea.y - 5.0;

        renderer.draw_text(
            canvas,
            yLabel_,
            Point{x, baseline},
            labelFontSize,
            black
        );
    }

    // Draw numerical tick labels for every axes object.
    for (const Axes& axes : axes_) {
        axes.drawTickLabels(canvas, renderer);
    }
}

void Figure::drawAxes(Canvas& canvas) const {
    const Color axisColor{60, 60, 60, 255};   // dark grey
    for (const Axes& axes : axes_) {
        axes.drawAxes(canvas, axisColor);      // Member 5's axis lines and tick marks
    }
}

} // namespace vizlib
