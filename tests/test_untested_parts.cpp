// Tests for code that no test ran before (found with a coverage report on
// 4 November 2026, see docs/bug-report.md). Each test names the function.
#include "doctest.h"

#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/ecdf.hpp"
#include "vizlib/figure.hpp"
#include "vizlib/legend.hpp"
#include "vizlib/text.hpp"

#include <stdexcept>
#include <vector>

namespace {
// true if any pixel inside the rectangle is darker than the background
bool has_ink(const vizlib::Canvas& canvas, int x0, int y0, int x1, int y1) {
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            if (canvas.get_pixel(x, y).r < 200) {
                return true;
            }
        }
    }
    return false;
}
}  // namespace

TEST_CASE("Axes::drawTickLabels writes numbers below and left of the plot area") {
    const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
    vizlib::Canvas canvas(300, 240, vizlib::Color{255, 255, 255, 255});
    vizlib::Axes axes;
    axes.setPlotArea({60, 20, 200, 160});   // bottom edge at y = 180
    axes.setDataRange(0, 10, 0, 10);

    CHECK_FALSE(has_ink(canvas, 0, 0, 300, 240));
    axes.drawTickLabels(canvas, text);
    CHECK(has_ink(canvas, 60, 181, 270, 215));   // x numbers under the plot
    CHECK(has_ink(canvas, 0, 15, 59, 185));      // y numbers left of it
    CHECK_FALSE(has_ink(canvas, 61, 21, 259, 179));   // nothing inside the plot area
}

TEST_CASE("Axes::drawTickLabels works on a log axis") {
    const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
    vizlib::Canvas canvas(300, 240, vizlib::Color{255, 255, 255, 255});
    vizlib::Axes axes;
    axes.setPlotArea({60, 20, 200, 160});
    axes.setDataRange(1, 1000, 0, 10);
    axes.setXScale(vizlib::ScaleType::Logarithmic);
    CHECK_NOTHROW(axes.drawTickLabels(canvas, text));
    CHECK(has_ink(canvas, 60, 181, 270, 215));
}

TEST_CASE("RugPlot::bounds pads the x range and uses 0 to 1 for y") {
    const vizlib::DataBounds b = vizlib::RugPlot(std::vector<double>{1, 3}).bounds();
    CHECK(b.x_min == doctest::Approx(0.9));
    CHECK(b.x_max == doctest::Approx(3.1));
    CHECK(b.y_min == doctest::Approx(0.0));
    CHECK(b.y_max == doctest::Approx(1.0));

    // one value: widened by 0.5 each side first, so the range is never 0
    const vizlib::DataBounds one = vizlib::RugPlot(std::vector<double>{2}).bounds();
    CHECK(one.x_min == doctest::Approx(1.45));
    CHECK(one.x_max == doctest::Approx(2.55));
}

TEST_CASE("ECDFPlot::bounds of equal values is never zero wide") {
    const vizlib::DataBounds b = vizlib::ECDFPlot(std::vector<double>{4, 4, 4}).bounds();
    CHECK(b.x_min < 4.0);
    CHECK(b.x_max > 4.0);
}

TEST_CASE("Figure::setSize and setMargins refuse sizes with no room to plot") {
    vizlib::Figure figure(400, 300);
    CHECK_THROWS_AS(figure.setSize(0, 300), std::invalid_argument);
    CHECK_THROWS_AS(figure.setSize(80, 300), std::invalid_argument);   // margins 60 + 30 wider than 80
    CHECK_NOTHROW(figure.setSize(500, 400));
    CHECK(figure.getWidth() == 500);
    CHECK(figure.getHeight() == 400);

    CHECK_THROWS_AS(figure.setMargins(-1, 10, 10, 10), std::invalid_argument);
    CHECK_THROWS_AS(figure.setMargins(300, 10, 300, 10), std::invalid_argument);
    CHECK_NOTHROW(figure.setMargins(10, 10, 10, 10));
    const vizlib::Rect area = figure.addAxes().getPlotArea();
    CHECK(area.x == doctest::Approx(10));
    CHECK(area.width == doctest::Approx(480));
}

TEST_CASE("Figure::getAxes throws for an index that does not exist") {
    vizlib::Figure figure(400, 300);
    CHECK_THROWS_AS(figure.getAxes(0), std::out_of_range);
    figure.addAxes();
    CHECK(figure.getAxesCount() == 1);
    CHECK_NOTHROW(figure.getAxes(0));
    const vizlib::Figure& read_only = figure;
    CHECK_THROWS_AS(read_only.getAxes(1), std::out_of_range);
}

TEST_CASE("Legend::clear removes every entry, and the position can be read back") {
    vizlib::Legend legend;
    legend.add_entry("a", vizlib::Color{255, 0, 0, 255});
    legend.add_entry("b", vizlib::Color{0, 0, 255, 255});
    REQUIRE(legend.get_entries().size() == 2);
    legend.clear();
    CHECK(legend.get_entries().empty());

    legend.set_position({25, 40});
    CHECK(legend.get_position().x == doctest::Approx(25));
    CHECK(legend.get_position().y == doctest::Approx(40));
}
