
#pragma once

#include "vizlib/axes.hpp"
#include "vizlib/geometry.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

#include <cstddef>
#include <deque>
#include <string>

namespace vizlib {

// figure.hpp - Figure (Member 5); title and labels by Member 8.
//
// A Figure is the whole image: its size, the margins around each plot, and
// one or more Axes. addAxes(area) takes a rectangle of the image, removes the
// margins, and gives the rest to a new Axes as its plot area. So two calls,
// one for the left half and one for the right half, give two plots side by
// side (as in pipeline.cpp).
class Figure {
private:
    // the image size in pixels
    int width_;
    int height_;

    // space kept around each plot area for tick numbers and labels:
    // left 60, top 30, right 30, bottom 50 pixels by default
    double leftMargin_;
    double topMargin_;
    double rightMargin_;
    double bottomMargin_;

    // std::deque, not std::vector: addAxes returns a reference to the new
    // Axes. A vector may move all its elements when it grows, which would
    // leave earlier references pointing at freed memory. A deque never moves
    // the elements it already has when you add at the end.
    std::deque<Axes> axes_;

    // Figure title and axis labels
    std::string title_;
    std::string xLabel_;
    std::string yLabel_;

public:
    // Throws std::invalid_argument if the size leaves no room inside the margins.
    Figure(int width = 800, int height = 600);

    void setSize(int width, int height);   // same check as the constructor

    // Throws for negative or infinite margins, or margins wider than the figure.
    void setMargins(double left, double top,
                    double right, double bottom);

    // Function overloading: the same name with different parameters.
    // addAxes() uses the whole figure; it simply calls addAxes(Rect) with it.
    // addAxes(area) uses part of the figure. Both return the new Axes.
    Axes& addAxes();
    Axes& addAxes(Rect area);

    // Throws std::out_of_range for an index that does not exist. The const
    // version lets a const Figure be read.
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

    // Draw the title and axis labels on a canvas
    void drawLabels(
        Canvas& canvas,
        const TextRenderer& renderer
    ) const;

    // Draw the axis lines and tick marks of every axes object
    // (see Axes::drawAxes). Call it after drawing the plots, so the
    // axes are on top.
    void drawAxes(Canvas& canvas) const;
};

} // namespace vizlib
