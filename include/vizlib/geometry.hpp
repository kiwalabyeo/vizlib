//include/vizlib/geometry.hpp (Point and Rect)

#pragma once

namespace vizlib {

// a position on the canvas (or in a plot)
struct Point {
    double x = 0;
    double y = 0;
};

// a rectangle: top-left corner plus its size
struct Rect {
    double x = 0;        // left edge
    double y = 0;        // top edge
    double width = 0;
    double height = 0;
};

}  // end of namespace vizlib
