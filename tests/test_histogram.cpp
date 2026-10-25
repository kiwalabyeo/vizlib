#include "doctest.h"

#include <vizlib/histogram.hpp>

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

    // edges 0..4 with counts 1, 2, 4, 0
    const std::vector<double> edges = {0, 1, 2, 3, 4};
    const std::vector<double> data = {0.5, 1.5, 1.5, 2.5, 2.5, 2.5, 2.5};
}

TEST_CASE("histogram_counts puts each value in the right bin")
{
    const auto counts = vizlib::histogram_counts(data, edges);
    REQUIRE(counts.size() == 4);
    CHECK(counts[0] == 1);
    CHECK(counts[1] == 2);
    CHECK(counts[2] == 4);
    CHECK(counts[3] == 0);
}

TEST_CASE("histogram_counts: left edge is inside a bin, only the last right edge is too")
{
    // 1 sits on the edge between bin 0 and 1, so it goes in bin 1.
    // 2 is the last edge, so it goes in the last bin instead of being dropped.
    const auto counts = vizlib::histogram_counts({0, 1, 2}, {0, 1, 2});
    CHECK(counts[0] == 1);
    CHECK(counts[1] == 2);
}

TEST_CASE("histogram_counts skips NaN and values outside the edges")
{
    const auto counts = vizlib::histogram_counts({-1, NAN, 0.5, 5}, {0, 1, 2});
    CHECK(counts[0] == 1);
    CHECK(counts[1] == 0);
}

TEST_CASE("histogram_counts rejects bad edges")
{
    CHECK_THROWS_AS(vizlib::histogram_counts({1}, {0}), std::invalid_argument);           // one edge
    CHECK_THROWS_AS(vizlib::histogram_counts({1}, {0, 2, 1}), std::invalid_argument);     // decreasing
    CHECK_THROWS_AS(vizlib::histogram_counts({1}, {0, 1, 1}), std::invalid_argument);     // repeated
    CHECK_THROWS_AS(vizlib::histogram_counts({1}, {0, INFINITY}), std::invalid_argument); // not finite
}

TEST_CASE("vertical histogram draws bars upwards with the right heights")
{
    vizlib::Canvas canvas(100, 100, white);
    vizlib::Axes axes;
    axes.setPlotArea({0, 0, 100, 100});
    vizlib::HistogramStyle style;
    style.color = red;

    // count axis runs 0..4.2, so a count of 1 is 23.8 px tall and 4 is 95.2 px
    vizlib::histogram(canvas, axes, data, edges, style);

    CHECK(same_color(canvas.get_pixel(12, 90), red));    // bin 0, inside the bar
    CHECK(same_color(canvas.get_pixel(12, 70), white));  // bin 0, above the bar
    CHECK(same_color(canvas.get_pixel(62, 10), red));    // bin 2, near its top
    CHECK(same_color(canvas.get_pixel(62, 3), white));   // bin 2, above the bar
    CHECK(same_color(canvas.get_pixel(87, 95), white));  // bin 3 is empty
}

TEST_CASE("horizontal histogram draws bars to the right with low bins at the bottom")
{
    vizlib::Canvas canvas(100, 100, white);
    vizlib::Axes axes;
    axes.setPlotArea({0, 0, 100, 100});
    vizlib::HistogramStyle style;
    style.color = red;
    style.orientation = vizlib::Orientation::Horizontal;

    vizlib::histogram(canvas, axes, data, edges, style);

    CHECK(same_color(canvas.get_pixel(10, 90), red));    // bin 0 is the bottom row of bars
    CHECK(same_color(canvas.get_pixel(30, 90), white));  // past the end of bin 0
    CHECK(same_color(canvas.get_pixel(90, 37), red));    // bin 2, near its end
    CHECK(same_color(canvas.get_pixel(97, 37), white));  // past the end of bin 2
    CHECK(same_color(canvas.get_pixel(10, 12), white));  // bin 3 (top) is empty
}

TEST_CASE("histogram sets the data range of the axes")
{
    vizlib::Canvas canvas(100, 100, white);
    vizlib::Axes axes;
    axes.setPlotArea({0, 0, 100, 100});

    vizlib::histogram(canvas, axes, data, edges);

    const vizlib::Point corner = axes.dataToPixel(4, 4.2);  // last edge, tallest bar + 5%
    CHECK(corner.x == doctest::Approx(100));
    CHECK(corner.y == doctest::Approx(0));
}

TEST_CASE("histogram rejects bad input")
{
    vizlib::Canvas canvas(10, 10, white);
    vizlib::Axes axes;
    axes.setPlotArea({0, 0, 10, 10});

    CHECK_THROWS_AS(vizlib::histogram(canvas, axes, {}, {0, 1}), std::invalid_argument);        // empty
    CHECK_THROWS_AS(vizlib::histogram(canvas, axes, {NAN, NAN}, {0, 1}), std::invalid_argument); // only NaN
    CHECK_THROWS_AS(vizlib::histogram(canvas, axes, {1}, {0}), std::invalid_argument);          // bad edges
}

TEST_CASE("auto_bin_edges spreads Freedman–Diaconis bins from min to max")
{
    // Freedman–Diaconis picks 3 bins for 1..10, spread evenly over the range
    const std::vector<double> e = vizlib::auto_bin_edges({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    REQUIRE(e.size() == 4);
    CHECK(e.front() == 1);
    CHECK(e[1] == doctest::Approx(4));
    CHECK(e.back() == 10);

    // order does not matter
    CHECK(vizlib::auto_bin_edges({10, 9, 8, 7, 6, 5, 4, 3, 2, 1}) == e);
}

TEST_CASE("auto_bin_edges handles an IQR of 0, equal values and NaN")
{
    // IQR is 0 but the values differ: Sturges' rule, ceil(log2(9)) + 1 = 5 bins
    const std::vector<double> sturges = vizlib::auto_bin_edges({1, 1, 1, 1, 1, 1, 1, 1, 100});
    CHECK(sturges.size() == 6);
    CHECK(sturges.front() == 1);
    CHECK(sturges.back() == 100);

    // every value the same: one bin of width 1 around it
    CHECK(vizlib::auto_bin_edges({5, 5, 5, 5}) == std::vector<double>{4.5, 5.5});

    // NaN is ignored, and only NaN throws
    CHECK(vizlib::auto_bin_edges({NAN, 2, 4}).front() == 2);
    CHECK_THROWS_AS(vizlib::auto_bin_edges({NAN, NAN}), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::auto_bin_edges({}), std::invalid_argument);
}

TEST_CASE("auto_bin_edges never loses a value")
{
    const std::vector<double> data = {3.2, -1.5, 7.75, 0, 0, 2.5, 9.1, 4.4, 4.4, 6};
    const auto counts = vizlib::histogram_counts(data, vizlib::auto_bin_edges(data));
    std::size_t total = 0;
    for (std::size_t c : counts)
        total += c;
    CHECK(total == data.size());
}
