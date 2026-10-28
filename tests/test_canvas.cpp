#include "doctest.h"
#include "vizlib/canvas.hpp"
#include <stdexcept>

using namespace vizlib;

// Colour has no == operator, so we compare the four numbers ourselves.
static bool same(Color a, Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

static const Color BG{255, 255, 255, 255};   // white background
static const Color RED{255, 0, 0, 255};

TEST_CASE("set_pixel then get_pixel") {
    Canvas cv(5, 5, BG);
    cv.set_pixel(2, 1, RED);
    CHECK(same(cv.get_pixel(2, 1), RED));   // the pixel we set
    CHECK(same(cv.get_pixel(1, 2), BG));    // a different pixel is untouched
}

TEST_CASE("pixels off the canvas") {
    Canvas cv(5, 5, BG);
    cv.set_pixel(-1, 0, RED);               // must not crash
    cv.set_pixel(5, 5, RED);                // must not crash
    CHECK_THROWS(cv.get_pixel(5, 0));       // reading off the canvas throws
}

TEST_CASE("a canvas of size zero throws") {
    CHECK_THROWS_AS(Canvas(0, 5, BG), std::invalid_argument);
    CHECK_THROWS_AS(Canvas(5, -1, BG), std::invalid_argument);
}

TEST_CASE("horizontal line") {
    Canvas cv(5, 5, BG);
    cv.draw_line({0, 0}, {4, 0}, RED);
    for (int x = 0; x < 5; ++x) CHECK(same(cv.get_pixel(x, 0), RED));
    CHECK(same(cv.get_pixel(2, 1), BG));    // the row below is untouched
}

TEST_CASE("diagonal line") {
    Canvas cv(5, 5, BG);
    cv.draw_line({0, 0}, {4, 4}, RED);
    for (int i = 0; i < 5; ++i) CHECK(same(cv.get_pixel(i, i), RED));
}

TEST_CASE("a line that runs off the canvas does not crash") {
    Canvas cv(5, 5, BG);
    cv.draw_line({-10, 2}, {20, 2}, RED);
    CHECK(same(cv.get_pixel(0, 2), RED));
    CHECK(same(cv.get_pixel(4, 2), RED));
}

TEST_CASE("thickness below 1 throws") {
    Canvas cv(5, 5, BG);
    CHECK_THROWS_AS(cv.draw_line({0, 0}, {4, 0}, RED, 0), std::invalid_argument);
}

TEST_CASE("fill_rect") {
    Canvas cv(5, 5, BG);
    cv.fill_rect({1, 1, 2, 2}, RED);        // covers (1,1) (2,1) (1,2) (2,2)
    CHECK(same(cv.get_pixel(1, 1), RED));
    CHECK(same(cv.get_pixel(2, 2), RED));
    CHECK(same(cv.get_pixel(0, 0), BG));
    CHECK(same(cv.get_pixel(3, 3), BG));
}

TEST_CASE("draw_rect is an outline only") {
    Canvas cv(5, 5, BG);
    cv.draw_rect({0, 0, 5, 5}, RED);
    CHECK(same(cv.get_pixel(0, 0), RED));
    CHECK(same(cv.get_pixel(4, 4), RED));
    CHECK(same(cv.get_pixel(0, 2), RED));
    CHECK(same(cv.get_pixel(2, 2), BG));    // the inside is empty
}

TEST_CASE("draw_circle is an outline only") {
    Canvas cv(11, 11, BG);
    cv.draw_circle({5, 5}, 3, RED);
    CHECK(same(cv.get_pixel(8, 5), RED));   // right
    CHECK(same(cv.get_pixel(2, 5), RED));   // left
    CHECK(same(cv.get_pixel(5, 2), RED));   // top
    CHECK(same(cv.get_pixel(5, 8), RED));   // bottom
    CHECK(same(cv.get_pixel(5, 5), BG));    // the centre is empty
}

TEST_CASE("fill_circle is solid") {
    Canvas cv(11, 11, BG);
    cv.fill_circle({5, 5}, 3, RED);
    CHECK(same(cv.get_pixel(5, 5), RED));   // the centre is filled
    CHECK(same(cv.get_pixel(8, 5), RED));   // the edge is filled
    CHECK(same(cv.get_pixel(0, 0), BG));    // far corner is untouched
}

TEST_CASE("a negative radius throws") {
    Canvas cv(11, 11, BG);
    CHECK_THROWS_AS(cv.draw_circle({5, 5}, -1, RED), std::invalid_argument);
    CHECK_THROWS_AS(cv.fill_circle({5, 5}, -1, RED), std::invalid_argument);
}