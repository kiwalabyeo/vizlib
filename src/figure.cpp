
#include "vizlib/figure.hpp"
#include <cmath>
#include <stdexcept>

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

}
