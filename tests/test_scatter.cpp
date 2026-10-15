#include "doctest.h"

#include <vizlib/scatter.hpp>

#include <cmath>
#include <stdexcept>
#include <vector>

namespace
{
    const vizlib::Color white{255, 255, 255, 255};
    const vizlib::Color red{255, 0, 0, 255};

    bool same_color(vizlib::Color a, vizlib::Color b)
    {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }
}

TEST_CASE("scatter draws a dot where the data point should land")
{
    vizlib::Canvas canvas(100, 100, white);
    vizlib::ScatterStyle style;
    style.color = red;
    style.radius = 2;

    // Range 0..10 plus 5% padding is -0.5..10.5, so (5, 5) is exactly the
    // middle of the 100 x 100 area: pixel (50, 50).
    vizlib::scatter(canvas, {0, 0, 100, 100}, {0, 5, 10}, {0, 5, 10}, style);

    CHECK(same_color(canvas.get_pixel(50, 50), red));
    CHECK(same_color(canvas.get_pixel(52, 50), red));    // on the edge of the dot
    CHECK(same_color(canvas.get_pixel(53, 50), white));  // just outside it
}

TEST_CASE("scatter puts large y values at the top")
{
    vizlib::Canvas canvas(100, 100, white);
    vizlib::ScatterStyle style;
    style.color = red;

    // (0, 0) is bottom left and (10, 10) top right, about 4.5 px from each edge
    vizlib::scatter(canvas, {0, 0, 100, 100}, {0, 10}, {0, 10}, style);

    CHECK(same_color(canvas.get_pixel(5, 95), red));
    CHECK(same_color(canvas.get_pixel(95, 5), red));
    CHECK(same_color(canvas.get_pixel(5, 5), white));
    CHECK(same_color(canvas.get_pixel(95, 95), white));
}

TEST_CASE("scatter draws inside the given area only")
{
    vizlib::Canvas canvas(200, 100, white);
    vizlib::ScatterStyle style;
    style.color = red;

    // a single point lands in the middle of the right half
    vizlib::scatter(canvas, {100, 0, 100, 100}, {3}, {7}, style);

    CHECK(same_color(canvas.get_pixel(150, 50), red));
    CHECK(same_color(canvas.get_pixel(50, 50), white));
}

TEST_CASE("scatter rejects bad input")
{
    vizlib::Canvas canvas(10, 10, white);
    const vizlib::Rect area{0, 0, 10, 10};

    CHECK_THROWS_AS(vizlib::scatter(canvas, area, {1, 2}, {1}), std::invalid_argument);  // different lengths
    CHECK_THROWS_AS(vizlib::scatter(canvas, area, {}, {}), std::invalid_argument);       // empty
    CHECK_THROWS_AS(vizlib::scatter(canvas, area, {NAN}, {1}), std::invalid_argument);   // nothing to draw
}

TEST_CASE("scatter skips points containing NaN")
{
    vizlib::Canvas canvas(100, 100, white);
    vizlib::ScatterStyle style;
    style.color = red;

    // Without the NaN point the range is still 0..10, so (5, 5) stays at (50, 50).
    // If the NaN point were drawn it would land somewhere else and break this.
    vizlib::scatter(canvas, {0, 0, 100, 100}, {0, 5, NAN, 10}, {0, 5, 3, 10}, style);

    CHECK(same_color(canvas.get_pixel(50, 50), red));
    int red_pixels = 0;
    for (int y = 0; y < 100; ++y)
        for (int x = 0; x < 100; ++x)
            if (same_color(canvas.get_pixel(x, y), red))
                ++red_pixels;

    vizlib::Canvas expected(100, 100, white);
    vizlib::scatter(expected, {0, 0, 100, 100}, {0, 5, 10}, {0, 5, 10}, style);
    int expected_red = 0;
    for (int y = 0; y < 100; ++y)
        for (int x = 0; x < 100; ++x)
            if (same_color(expected.get_pixel(x, y), red))
                ++expected_red;

    CHECK(red_pixels == expected_red);
}
