
#include "vizlib/figure.hpp"

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

// Figure title
void Figure::setTitle(const std::string& title) {
    title_ = title;
}

const std::string& Figure::getTitle() const {
    return title_;
}

// Horizontal axis label
void Figure::setXLabel(const std::string& label) {
    xLabel_ = label;
}

const std::string& Figure::getXLabel() const {
    return xLabel_;
}

// Vertical axis label
void Figure::setYLabel(const std::string& label) {
    yLabel_ = label;
}

const std::string& Figure::getYLabel() const {
    return yLabel_;
}

// Draw the figure title and axis labels.
void Figure::drawLabels(
    Canvas& canvas,
    const TextRenderer& renderer
) const {
    const Color black{0, 0, 0};

    // Draw the title, centered near the top of the figure.
    if (!title_.empty()) {
        constexpr double titleFontSize = 18.0;

        const TextMetrics metrics =
            renderer.measure_text(title_, titleFontSize);

        const double x =
            (width_ - metrics.width) / 2.0;

        const double baseline =
            topMargin_ - 5.0;

        renderer.draw_text(
            canvas,
            title_,
            Point{x, baseline},
            titleFontSize,
            black
        );
    }

    // Draw the X-axis label, centered near the bottom.
    if (!xLabel_.empty()) {
        constexpr double labelFontSize = 12.0;

        const TextMetrics metrics =
            renderer.measure_text(xLabel_, labelFontSize);

        const double x =
            (width_ - metrics.width) / 2.0;

        const double baseline =
            height_ - bottomMargin_ / 2.0 +
            labelFontSize / 2.0;

        renderer.draw_text(
            canvas,
            xLabel_,
            Point{x, baseline},
            labelFontSize,
            black
        );
    }

    // Draw the Y-axis label horizontally on the left.
    // Text rotation is not currently supported.
    if (!yLabel_.empty()) {
        constexpr double labelFontSize = 12.0;

        const TextMetrics metrics =
            renderer.measure_text(yLabel_, labelFontSize);

        const double x = 5.0;

        const double baseline =
            topMargin_ +
            (height_ - topMargin_ - bottomMargin_) / 2.0 +
            metrics.height / 2.0;

        renderer.draw_text(
            canvas,
            yLabel_,
            Point{x, baseline},
            labelFontSize,
            black
        );
    }
}

} // namespace vizlib
