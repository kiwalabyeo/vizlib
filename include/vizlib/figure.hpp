
#pragma once

#include "vizlib/axes.hpp"
#include "vizlib/geometry.hpp"
#include <deque>
#include <cstddef>

namespace vizlib {

class Figure {
private:
    int width_;
    int height_;

    double leftMargin_;
    double topMargin_;
    double rightMargin_;
    double bottomMargin_;

    std::deque<Axes> axes_;

public:
    Figure(int width = 800, int height = 600);

    void setSize(int width, int height);

    void setMargins(double left, double top,
                    double right, double bottom);

    Axes& addAxes();
    Axes& addAxes(Rect area);

    Axes& getAxes(std::size_t index);
    const Axes& getAxes(std::size_t index) const;

    std::size_t getAxesCount() const;

    int getWidth() const;
    int getHeight() const;
};

}
