#pragma once  // makes sure only one copy of this file is included
#include <vector>
#include "vizlib/color.hpp"
#include "vizlib/geometry.hpp"

namespace vizlib {

class Canvas {
private:
    int width_;
    int height_;
    std::vector<Color> pixels_;   // the list that holds the colour of every pixel

public:
    // Creates a Canvas with the specified width, height and background colour.
    // Throws std::invalid_argument if width or height is not positive.
    Canvas(int width, int height, Color background);

    // Returns the width of the Canvas in pixels.
    int width() const;

    // Returns the height of the Canvas in pixels.
    int height() const;

    // Changes the colour of the pixel at (x, y).
    // Does nothing if the coordinates are outside the Canvas.
    void set_pixel(int x, int y, Color c);

    // Returns the colour of the pixel at (x, y).
// Throws std::out_of_range if the coordinates are outside the Canvas.
    Color get_pixel(int x, int y) const;

    
    // Draws a line between two points using Bresenham's algorithm.
    // thickness specifies the width of the line in pixels.
    // Throws std::invalid_argument for invalid coordinates or thickness.
    void draw_line(Point from, Point to, Color c, int thickness = 1);
    
    // Draws the outline of a rectangle.
    // A rectangle with non-positive width or height draws nothing.
    void draw_rect(Rect r, Color c);

    // Fills the interior of a rectangle.
    // A rectangle with non-positive width or height draws nothing.
    void fill_rect(Rect r, Color c);

    // Draws the outline of a circle using the midpoint circle algorithm.
    // Throws std::invalid_argument if the radius is negative or an input is invalid.
    void draw_circle(Point center, double radius, Color c);

    // Fills the interior of a circle.
    // Throws std::invalid_argument if the radius is negative or an input is invalid.
    void fill_circle(Point center, double radius, Color c);
};

}  // end of namespace vizlib
