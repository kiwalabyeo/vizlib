// Converted from a main() program that printed values into doctest checks,
// so CI runs it. Same scale, figure and points as the original.
#include "doctest.h"

#include "vizlib/axes.hpp"
#include "vizlib/figure.hpp"
#include "vizlib/scale.hpp"

TEST_CASE("LinearScale maps data to pixels and back") {
    vizlib::LinearScale scale(0, 100, 50, 450);

    CHECK(scale.dataToPixel(50) == doctest::Approx(250));
    CHECK(scale.pixelToData(250) == doctest::Approx(50));
}

TEST_CASE("Figure axes put the middle of the data in the middle of the plot area") {
    vizlib::Figure figure(800, 600);
    vizlib::Axes& axes = figure.addAxes();
    axes.setDataRange(0, 100, 0, 50);

    const vizlib::Rect area = axes.getPlotArea();
    const vizlib::Point point = axes.dataToPixel(50, 25);

    CHECK(point.x == doctest::Approx(area.x + area.width / 2));
    CHECK(point.y == doctest::Approx(area.y + area.height / 2));
    CHECK(figure.getAxesCount() == 1);
}

// ---- Axis lines and tick marks (added 3 November) ----

#include "vizlib/canvas.hpp"
#include "vizlib/ticks.hpp"

#include <cmath>

#include <stdexcept>
#include <vector>

namespace {

const vizlib::Color white{255, 255, 255, 255};
const vizlib::Color red{255, 0, 0, 255};

bool is_red(const vizlib::Canvas& c, int x, int y) {
    const vizlib::Color p = c.get_pixel(x, y);
    return p.r == 255 && p.g == 0 && p.b == 0;
}

// plot area from (20, 10) to (120, 110) on a 140 x 130 canvas, data 0..10 on both axes
vizlib::Axes test_axes() {
    vizlib::Axes axes;
    axes.setPlotArea({20, 10, 100, 100});
    axes.setDataRange(0, 10, 0, 10);
    return axes;
}

}  // namespace

TEST_CASE("xTicks and yTicks are the nice ticks of each axis") {
    const vizlib::Axes axes = test_axes();
    CHECK(axes.xTicks() == vizlib::generateTicks(0, 10));
    CHECK(axes.yTicks() == vizlib::generateTicks(0, 10));

    vizlib::Axes log_axes;
    log_axes.setPlotArea({0, 0, 100, 100});
    log_axes.setDataRange(1, 1000, 0, 1);
    log_axes.setXScale(vizlib::ScaleType::Logarithmic);
    CHECK(log_axes.xTicks() == std::vector<double>{1, 10, 100, 1000});
}

TEST_CASE("drawAxisLines draws the x axis at the bottom and the y axis on the left") {
    vizlib::Canvas canvas(140, 130, white);
    test_axes().drawAxisLines(canvas, red);

    CHECK(is_red(canvas, 70, 110));    // on the x axis (bottom edge, y = 110)
    CHECK(is_red(canvas, 20, 60));     // on the y axis (left edge, x = 20)
    CHECK_FALSE(is_red(canvas, 70, 60));   // inside the plot area stays empty
    CHECK_FALSE(is_red(canvas, 120, 60));  // no line on the right edge
    CHECK_FALSE(is_red(canvas, 70, 10));   // no line on the top edge
}

TEST_CASE("drawAxisLines puts a tick mark outside the axes at every tick") {
    vizlib::Canvas canvas(140, 130, white);
    const vizlib::Axes axes = test_axes();
    axes.drawAxisLines(canvas, red, 5);

    for (double tick : axes.xTicks()) {
        const int x = static_cast<int>(std::lround(axes.dataToPixel(tick, 0).x));
        CHECK(is_red(canvas, x, 114));   // below the x axis
    }
    for (double tick : axes.yTicks()) {
        const int y = static_cast<int>(std::lround(axes.dataToPixel(0, tick).y));
        CHECK(is_red(canvas, 16, y));    // left of the y axis
    }
    // between two ticks there is no mark (ticks are at 0, 2, 4, ... so 1 has none)
    const int between = static_cast<int>(std::lround(axes.dataToPixel(1, 0).x));
    CHECK_FALSE(is_red(canvas, between, 114));
}

TEST_CASE("drawAxisLines with tick length 0 draws only the lines, and rejects a negative length") {
    vizlib::Canvas canvas(140, 130, white);
    test_axes().drawAxisLines(canvas, red, 0);
    CHECK(is_red(canvas, 70, 110));
    CHECK_FALSE(is_red(canvas, 20, 114));   // no tick mark under the corner

    CHECK_THROWS_AS(test_axes().drawAxisLines(canvas, red, -1), std::invalid_argument);
}

TEST_CASE("Figure::drawAxes draws the axes of every Axes object") {
    vizlib::Figure figure(400, 200);
    vizlib::Axes& left = figure.addAxes(vizlib::Rect{0, 0, 200, 200});
    vizlib::Axes& right = figure.addAxes(vizlib::Rect{200, 0, 200, 200});
    left.setDataRange(0, 1, 0, 1);
    right.setDataRange(0, 1, 0, 1);

    vizlib::Canvas canvas(400, 200, white);
    figure.drawAxes(canvas);

    for (const vizlib::Axes* axes : {&left, &right}) {
        const vizlib::Rect a = axes->getPlotArea();
        const vizlib::Color p = canvas.get_pixel(static_cast<int>(a.x + a.width / 2),
                                                 static_cast<int>(a.y + a.height));
        CHECK_FALSE((p.r == 255 && p.g == 255 && p.b == 255));   // the x axis is drawn
    }
}
