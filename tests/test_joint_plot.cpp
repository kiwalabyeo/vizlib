#include "doctest.h"

#include <vizlib/joint_plot.hpp>
#include <vizlib/kde_plot.hpp>

#include <cmath>
#include <memory>
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

    int count_color(const vizlib::Canvas& canvas, vizlib::Rect r, vizlib::Color c)
    {
        int n = 0;
        for (int y = static_cast<int>(r.y); y < r.y + r.height; ++y)
            for (int x = static_cast<int>(r.x); x < r.x + r.width; ++x)
                if (same_color(canvas.get_pixel(x, y), c))
                    ++n;
        return n;
    }

    // A Plot that draws nothing and remembers how it was used, to check that
    // JointPlot only relies on the Plot interface.
    class FakePlot : public vizlib::Plot
    {
    public:
        vizlib::DataBounds b;
        mutable int draw_calls = 0;
        mutable vizlib::Rect drawn_area;

        explicit FakePlot(vizlib::DataBounds bounds) : b(bounds) {}

        vizlib::DataBounds bounds() const override { return b; }
        void draw(vizlib::Canvas&, const vizlib::Axes& axes) const override
        {
            ++draw_calls;
            drawn_area = axes.getPlotArea();
        }
    };
}

TEST_CASE("ScatterPlot bounds are the data range plus 5% padding")
{
    const vizlib::ScatterPlot plot({0, 10, NAN}, {0, 20, 100});
    const vizlib::DataBounds b = plot.bounds();
    CHECK(b.x_min == doctest::Approx(-0.5));
    CHECK(b.x_max == doctest::Approx(10.5));
    CHECK(b.y_min == doctest::Approx(-1));    // the NaN point (y = 100) is ignored
    CHECK(b.y_max == doctest::Approx(21));
}

TEST_CASE("scatter through an Axes matches scatter through a Rect")
{
    vizlib::Canvas with_rect(100, 100, white);
    vizlib::Canvas with_axes(100, 100, white);
    const std::vector<double> x = {1, 4, 2, 8};
    const std::vector<double> y = {3, 1, 7, 5};

    vizlib::scatter(with_rect, {10, 10, 80, 80}, x, y);
    vizlib::Axes axes;
    axes.setPlotArea({10, 10, 80, 80});
    vizlib::scatter(with_axes, axes, x, y);

    bool same = true;
    for (int py = 0; py < 100; ++py)
        for (int px = 0; px < 100; ++px)
            same = same && same_color(with_rect.get_pixel(px, py), with_axes.get_pixel(px, py));
    CHECK(same);
}

TEST_CASE("HistogramPlot bounds swap with the orientation")
{
    vizlib::HistogramStyle style;
    const vizlib::HistogramPlot vertical({0.5, 1.5, 1.5}, {0, 1, 2}, style);
    CHECK(vertical.counts() == std::vector<std::size_t>{1, 2});
    vizlib::DataBounds b = vertical.bounds();
    CHECK(b.x_min == 0);
    CHECK(b.x_max == 2);
    CHECK(b.y_min == 0);
    CHECK(b.y_max == doctest::Approx(2.1));

    style.orientation = vizlib::Orientation::Horizontal;
    const vizlib::HistogramPlot horizontal({0.5, 1.5, 1.5}, {0, 1, 2}, style);
    b = horizontal.bounds();
    CHECK(b.x_max == doctest::Approx(2.1));
    CHECK(b.y_min == 0);
    CHECK(b.y_max == 2);
}

TEST_CASE("KdePlot bounds and validation")
{
    const vizlib::KdePlot plot({0, 1, 2}, {0.1, 0.4, 0.2});
    const vizlib::DataBounds b = plot.bounds();
    CHECK(b.x_min == 0);
    CHECK(b.x_max == 2);
    CHECK(b.y_max == doctest::Approx(0.42));

    CHECK_THROWS_AS(vizlib::KdePlot({0, 1}, {0.1}), std::invalid_argument);           // lengths differ
    CHECK_THROWS_AS(vizlib::KdePlot({0}, {0.1}), std::invalid_argument);              // one point
    CHECK_THROWS_AS(vizlib::KdePlot({1, 0}, {0.1, 0.1}), std::invalid_argument);      // grid decreasing
    CHECK_THROWS_AS(vizlib::KdePlot({0, 1}, {0.1, -0.1}), std::invalid_argument);     // negative density
    CHECK_THROWS_AS(vizlib::KdePlot({0, 1}, {0.1, NAN}), std::invalid_argument);      // NaN density
}

TEST_CASE("Horizontal KdePlot puts the grid on y and the density on x")
{
    vizlib::KdeStyle style;
    style.color = red;
    style.thickness = 1;
    style.orientation = vizlib::Orientation::Horizontal;
    const vizlib::KdePlot plot({0, 10}, {0, 4}, style);

    const vizlib::DataBounds b = plot.bounds();
    CHECK(b.x_min == 0);
    CHECK(b.x_max == doctest::Approx(4.2));
    CHECK(b.y_min == 0);
    CHECK(b.y_max == 10);

    vizlib::Canvas canvas(100, 100, white);
    vizlib::Axes axes;
    axes.setPlotArea({0, 0, 100, 100});
    axes.setDataRange(0, 10, 0, 10);
    plot.draw(canvas, axes);

    // grid 0 -> 10 runs bottom to top, density 0 -> 4 runs left to 40% across
    CHECK(same_color(canvas.get_pixel(0, 99), red));
    CHECK(same_color(canvas.get_pixel(20, 50), red));
    CHECK(same_color(canvas.get_pixel(50, 20), white));   // where a Vertical curve would be
}

TEST_CASE("KdePlot draws a line through its points")
{
    vizlib::Canvas canvas(100, 100, white);
    vizlib::KdeStyle style;
    style.color = red;
    style.thickness = 1;
    const vizlib::KdePlot plot({0, 10}, {0, 10}, style);

    vizlib::Axes axes;
    axes.setPlotArea({0, 0, 100, 100});
    axes.setDataRange(0, 10, 0, 10);
    plot.draw(canvas, axes);

    // the diagonal from bottom left to top right
    CHECK(same_color(canvas.get_pixel(50, 50), red));
    CHECK(same_color(canvas.get_pixel(20, 80), red));
    CHECK(same_color(canvas.get_pixel(20, 20), white));
}

TEST_CASE("JointPlot works with any Plot through the base class")
{
    auto center = std::make_unique<FakePlot>(vizlib::DataBounds{0, 10, 0, 10});
    auto top = std::make_unique<FakePlot>(vizlib::DataBounds{0, 10, 0, 5});
    auto right = std::make_unique<FakePlot>(vizlib::DataBounds{0, 5, 0, 10});
    const FakePlot* c = center.get();
    const FakePlot* t = top.get();
    const FakePlot* r = right.get();

    const vizlib::JointPlot joint(std::move(center), std::move(top), std::move(right));
    vizlib::Canvas canvas(100, 100, white);
    joint.draw(canvas, {0, 0, 100, 100}, {0.2, 0});

    CHECK(c->draw_calls == 1);
    CHECK(t->draw_calls == 1);
    CHECK(r->draw_calls == 1);
    // with no gap: top is 20 px tall, right is 20 px wide, centre gets the rest
    CHECK(c->drawn_area.width == doctest::Approx(80));
    CHECK(c->drawn_area.height == doctest::Approx(80));
    CHECK(t->drawn_area.y == doctest::Approx(0));
    CHECK(r->drawn_area.x == doctest::Approx(80));
}

TEST_CASE("JointPlot margins share the centre panel's scales")
{
    // the top margin's bins reach further (-1..12) than the scatter data (0..10)
    const vizlib::JointPlot joint = vizlib::JointPlot::with_histograms(
        {0, 5, 10}, {0, 5, 10}, {-1, 4, 8, 12}, {0, 5, 10});
    const vizlib::JointAxes axes = joint.layout({0, 0, 200, 200});

    // the same x lands in the same pixel column in centre and top
    for (double x : {-1.0, 3.0, 12.0})
        CHECK(axes.center.dataToPixel(x, 0).x == doctest::Approx(axes.top.dataToPixel(x, 0).x));
    // the same y lands in the same pixel row in centre and right
    for (double y : {0.0, 2.5, 10.0})
        CHECK(axes.center.dataToPixel(0, y).y == doctest::Approx(axes.right.dataToPixel(0, y).y));

    // the shared x range is wide enough for both: -1 is the left edge of the panel
    CHECK(axes.center.dataToPixel(-1, 0).x == doctest::Approx(axes.center.getPlotArea().x));
}

TEST_CASE("JointPlot::with_histograms draws in all three panels")
{
    vizlib::ScatterStyle style;
    style.color = red;
    const vizlib::JointPlot joint = vizlib::JointPlot::with_histograms(
        {1, 2, 2, 3, 4}, {4, 3, 3, 2, 1}, {0, 1, 2, 3, 4, 5}, {0, 1, 2, 3, 4, 5}, style);

    vizlib::Canvas canvas(200, 200, white);
    joint.draw(canvas, {0, 0, 200, 200});
    const vizlib::JointAxes axes = joint.layout({0, 0, 200, 200});

    CHECK(count_color(canvas, axes.center.getPlotArea(), red) > 0);
    CHECK(count_color(canvas, axes.top.getPlotArea(), red) > 0);
    CHECK(count_color(canvas, axes.right.getPlotArea(), red) > 0);
    // the top-right corner belongs to no panel and stays empty
    CHECK(count_color(canvas, {170, 0, 30, 30}, red) == 0);
}

TEST_CASE("JointPlot rejects bad input")
{
    CHECK_THROWS_AS(vizlib::JointPlot(nullptr, nullptr, nullptr), std::invalid_argument);

    const vizlib::JointPlot joint = vizlib::JointPlot::with_histograms({1, 2}, {1, 2}, {0, 3}, {0, 3});
    CHECK_THROWS_AS(joint.layout({0, 0, 100, 100}, {0, 6}), std::invalid_argument);    // ratio 0
    CHECK_THROWS_AS(joint.layout({0, 0, 100, 100}, {1.5, 6}), std::invalid_argument);  // ratio > 1
    CHECK_THROWS_AS(joint.layout({0, 0, 100, 100}, {0.2, 90}), std::invalid_argument); // gap too big

    // bad data is caught by the panel that receives it
    CHECK_THROWS_AS(vizlib::JointPlot::with_histograms({1, 2}, {1}, {0, 3}, {0, 3}), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::JointPlot::with_histograms({1, 2}, {1, 2}, {3, 0}, {0, 3}), std::invalid_argument);
}
