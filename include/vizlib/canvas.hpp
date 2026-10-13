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
    Canvas(int width, int height, Color background);

    int width() const;
    int height() const;

    void set_pixel(int x, int y, Color c);
    Color get_pixel(int x, int y) const;
    void draw_line(Point from, Point to, Color c, int thickness = 1);
    void draw_rect(Rect r, Color c);
    void fill_rect(Rect r, Color c);
    void draw_circle(Point center, double radius, Color c);
    void fill_circle(Point center, double radius, Color c);
};

}  // end of namespace vizlib
