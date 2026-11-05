// Converted from a main() program with assert() into doctest checks.
// Same values as the original.
#include "doctest.h"

#include "vizlib/axes.hpp"
#include "vizlib/ticks.hpp"

#include <stdexcept>
#include <vector>

using namespace vizlib;

TEST_CASE("Logarithmic x axis maps data to pixels and back") {
    Axes axes;
    axes.setPlotArea(Rect{10.0, 20.0, 100.0, 200.0});
    axes.setDataRange(1.0, 1000.0, 0.0, 10.0);
    axes.setXScale(ScaleType::Logarithmic);

    const Point pixel = axes.dataToPixel(10.0, 5.0);
    CHECK(pixel.x == doctest::Approx(43.3333333333));   // 10 is one third of the way from 1 to 1000
    CHECK(pixel.y == doctest::Approx(120.0));

    const Point data = axes.pixelToData(pixel.x, pixel.y);
    CHECK(data.x == doctest::Approx(10.0));
    CHECK(data.y == doctest::Approx(5.0));
}

TEST_CASE("Logarithmic y axis maps data to pixels") {
    Axes axes;
    axes.setPlotArea(Rect{10.0, 20.0, 100.0, 200.0});
    axes.setDataRange(0.0, 10.0, 1.0, 1000.0);
    axes.setYScale(ScaleType::Logarithmic);

    const Point pixel = axes.dataToPixel(5.0, 10.0);
    CHECK(pixel.x == doctest::Approx(60.0));
    CHECK(pixel.y == doctest::Approx(153.3333333333));
}

TEST_CASE("generateLogTicks gives the powers of 10") {
    CHECK(generateLogTicks(1.0, 1000.0) == std::vector<double>{1.0, 10.0, 100.0, 1000.0});
    CHECK_THROWS_AS(generateLogTicks(-1.0, 100.0), std::invalid_argument);
}

TEST_CASE("Logarithmic axis rejects zero and negative values") {
    Axes axes;
    axes.setPlotArea(Rect{10.0, 20.0, 100.0, 200.0});
    axes.setDataRange(1.0, 1000.0, 0.0, 10.0);
    axes.setXScale(ScaleType::Logarithmic);

    CHECK_THROWS_AS(axes.dataToPixel(0.0, 5.0), std::invalid_argument);
    CHECK_THROWS_AS(axes.dataToPixel(-5.0, 5.0), std::invalid_argument);
}
