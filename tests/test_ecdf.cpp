// Crystal's ECDF tests (from tests_ecdf.cpp), rewritten for doctest so they
// run with the rest of the suite in CTest and CI.
#include "doctest.h"

#include <vizlib/ecdf.hpp>

#include <vector>

TEST_CASE("ECDF of an empty vector is empty")
{
    vizlib::ECDFPlot ecdf;
    CHECK(ecdf.compute_coordinates({}).empty());
}

TEST_CASE("ECDF sorts the data and steps up by 1/n")
{
    vizlib::ECDFPlot ecdf;
    const std::vector<vizlib::Point> result = ecdf.compute_coordinates({3.0, 1.0, 2.0});

    // Expected sorted order: 1.0, 2.0, 3.0
    // Expected y values: 1/3, 2/3, 3/3 (1.0)
    REQUIRE(result.size() == 3);
    CHECK(result[0].x == 1.0);
    CHECK(result[0].y == doctest::Approx(1.0 / 3.0));
    CHECK(result[1].x == 2.0);
    CHECK(result[1].y == doctest::Approx(2.0 / 3.0));
    CHECK(result[2].x == 3.0);
    CHECK(result[2].y == doctest::Approx(1.0));
}

// ---- ECDFPlot and RugPlot as Plots (added 3 November) ----

#include "numeric_types.hpp"

#include <vizlib/canvas.hpp>
#include <vizlib/plot.hpp>

#include <cmath>
#include <memory>
#include <stdexcept>

namespace
{
    const vizlib::Color white{255, 255, 255, 255};
    const vizlib::Color red{255, 0, 0, 255};

    bool is_red(const vizlib::Canvas& c, int x, int y)
    {
        const vizlib::Color p = c.get_pixel(x, y);
        return p.r == 255 && p.g == 0 && p.b == 0;
    }

    // axes filling a 101 x 101 canvas: data 0..1 on x maps to pixels 0..100,
    // data 0..1 on y maps to pixels 100..0 (y grows upwards in the data)
    vizlib::Axes unit_axes()
    {
        vizlib::Axes axes;
        axes.setPlotArea({0, 0, 100, 100});
        axes.setDataRange(0, 1, 0, 1);
        return axes;
    }
}

TEST_CASE("compute_coordinates leaves NaN out")
{
    vizlib::ECDFPlot ecdf;
    const auto result = ecdf.compute_coordinates({2.0, NAN, 1.0});

    REQUIRE(result.size() == 2);
    CHECK(result[0].x == 1.0);
    CHECK(result[0].y == doctest::Approx(0.5));
    CHECK(result[1].x == 2.0);
    CHECK(result[1].y == doctest::Approx(1.0));
}

TEST_CASE("ECDFPlot keeps its coordinates and has padded bounds")
{
    const vizlib::ECDFPlot plot({3.0, 1.0, 2.0});

    REQUIRE(plot.points().size() == 3);
    CHECK(plot.points().front().x == 1.0);
    CHECK(plot.bounds().x_min == doctest::Approx(1.0 - 0.1));   // 5% of a span of 2
    CHECK(plot.bounds().x_max == doctest::Approx(3.0 + 0.1));
    CHECK(plot.bounds().y_min == 0.0);
    CHECK(plot.bounds().y_max == doctest::Approx(1.05));
}

TEST_CASE("ECDFPlot rejects data it cannot draw")
{
    CHECK_THROWS_AS(vizlib::ECDFPlot(std::vector<double>{}), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::ECDFPlot(std::vector<double>{NAN, NAN}), std::invalid_argument);

    vizlib::EcdfStyle thin;
    thin.thickness = 0;
    CHECK_THROWS_AS(vizlib::ECDFPlot(std::vector<double>{1, 2}, thin), std::invalid_argument);
}

TEST_CASE("A default ECDFPlot draws nothing")
{
    vizlib::Canvas canvas(101, 101, white);
    const vizlib::ECDFPlot empty;
    empty.draw(canvas, unit_axes());

    for (int y = 0; y <= 100; ++y)
        for (int x = 0; x <= 100; ++x)
            REQUIRE_FALSE(is_red(canvas, x, y));
}

TEST_CASE("ECDFPlot draws steps: up at each value, flat in between")
{
    vizlib::EcdfStyle style;
    style.color = red;
    style.thickness = 1;
    const vizlib::ECDFPlot plot({0.25, 0.75}, style);   // jumps to 0.5 at x=0.25 and to 1 at x=0.75

    vizlib::Canvas canvas(101, 101, white);
    plot.draw(canvas, unit_axes());

    // up at x = 0.25 (pixel 25), from y = 0 (pixel 100) to y = 0.5 (pixel 50)
    CHECK(is_red(canvas, 25, 90));
    CHECK(is_red(canvas, 25, 60));
    // flat at y = 0.5 between the two values
    CHECK(is_red(canvas, 50, 50));
    // up at x = 0.75 from y = 0.5 to y = 1 (pixel 0)
    CHECK(is_red(canvas, 75, 25));
    // no diagonal: halfway along a straight line from (25, 100) to (75, 0) stays empty
    CHECK_FALSE(is_red(canvas, 50, 75));
    // nothing before the first value
    CHECK_FALSE(is_red(canvas, 10, 99));
}

TEST_CASE("ECDFPlot works through a Plot pointer")
{
    std::unique_ptr<vizlib::Plot> plot = std::make_unique<vizlib::ECDFPlot>(std::vector<double>{1, 2, 3});
    CHECK(plot->bounds().y_max == doctest::Approx(1.05));
}

TEST_CASE_TEMPLATE("ECDFPlot gives the same steps for every number type", T, VIZLIB_NUMERIC_TYPES)
{
    const vizlib::ECDFPlot plot(std::vector<T>{T(5), T(1), T(3), T(3)});
    const vizlib::ECDFPlot expected(std::vector<double>{5, 1, 3, 3});

    REQUIRE(plot.points().size() == expected.points().size());
    for (std::size_t i = 0; i < plot.points().size(); ++i)
    {
        CHECK(plot.points()[i].x == expected.points()[i].x);
        CHECK(plot.points()[i].y == doctest::Approx(expected.points()[i].y));
    }
}

TEST_CASE("RugPlot draws one mark per value on the bottom edge")
{
    vizlib::RugStyle style;
    style.color = red;
    style.height = 10;
    const vizlib::RugPlot rug({0.2, 0.6, NAN}, style);   // NaN is left out

    vizlib::Canvas canvas(101, 101, white);
    rug.draw(canvas, unit_axes());

    CHECK(is_red(canvas, 20, 100));   // standing on the bottom edge
    CHECK(is_red(canvas, 20, 91));    // 10 pixels tall
    CHECK_FALSE(is_red(canvas, 20, 85));
    CHECK(is_red(canvas, 60, 95));
    CHECK_FALSE(is_red(canvas, 40, 95));   // nothing between the values
}

TEST_CASE("RugPlot works on a logarithmic y axis")
{
    vizlib::Axes axes;
    axes.setPlotArea({0, 0, 100, 100});
    axes.setDataRange(0, 1, 1, 100);
    axes.setYScale(vizlib::ScaleType::Logarithmic);

    vizlib::Canvas canvas(101, 101, white);
    const vizlib::RugPlot rug({0.5});
    CHECK_NOTHROW(rug.draw(canvas, axes));
}

TEST_CASE("RugPlot rejects bad input")
{
    CHECK_THROWS_AS(vizlib::RugPlot(std::vector<double>{}), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::RugPlot(std::vector<double>{NAN}), std::invalid_argument);
    vizlib::RugStyle flat;
    flat.height = 0;
    CHECK_THROWS_AS(vizlib::RugPlot(std::vector<double>{1}, flat), std::invalid_argument);
}
