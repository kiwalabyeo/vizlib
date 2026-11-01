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
