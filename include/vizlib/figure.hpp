
#pragma once

#include "vizlib/axes.hpp"
#include "vizlib/geometry.hpp"
#include <deque>
#include <cstddef>
#include <string>

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

    // Figure title and axis labels
    std::string title_;
    std::string xLabel_;
    std::string yLabel_;

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

    // Set and retrieve the figure title
    void setTitle(const std::string& title);
    const std::string& getTitle() const;

    // Set and retrieve the horizontal axis label
    void setXLabel(const std::string& label);
    const std::string& getXLabel() const;

    // Set and retrieve the vertical axis label
    void setYLabel(const std::string& label);
    const std::string& getYLabel() const;
};

}
