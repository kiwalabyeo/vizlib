// Tests for the templated scatter, histogram and joint plot (Member 4).
//
// Each TEST_CASE_TEMPLATE runs once for every type in VIZLIB_NUMERIC_TYPES
// (short, int, long, long long, unsigned int, float, double). The data is made of
// small whole numbers so every type can hold it exactly, and the result must
// match what the double version gives.

#include "doctest.h"
#include "numeric_types.hpp"

#include "vizlib/canvas.hpp"
#include "vizlib/histogram.hpp"
#include "vizlib/joint_plot.hpp"
#include "vizlib/scatter.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

namespace {

const vizlib::Color white{255, 255, 255, 255};

// true if both canvases have the same size and the same colour at every pixel
bool same_pixels(const vizlib::Canvas& a, const vizlib::Canvas& b) {
    if (a.width() != b.width() || a.height() != b.height()) {
        return false;
    }
    for (int y = 0; y < a.height(); ++y) {
        for (int x = 0; x < a.width(); ++x) {
            const vizlib::Color p = a.get_pixel(x, y);
            const vizlib::Color q = b.get_pixel(x, y);
            if (p.r != q.r || p.g != q.g || p.b != q.b || p.a != q.a) {
                return false;
            }
        }
    }
    return true;
}

// the same values as a vector of any type
template <typename T>
std::vector<T> as(const std::vector<int>& values) {
    return std::vector<T>(values.begin(), values.end());
}

const std::vector<int> xs{1, 2, 2, 3, 3, 3, 4, 4, 5, 9};
const std::vector<int> ys{2, 4, 3, 6, 5, 7, 8, 6, 9, 12};

}  // namespace

// The concept is checked at compile time: ScatterPlot<bool> cannot even be named.
// (The check has to sit inside a concept, so a failure gives false instead of an error.)
template <typename TX, typename TY = TX>
concept can_make_scatter = requires { typename vizlib::ScatterPlot<TX, TY>; };

static_assert(can_make_scatter<int, float>);
static_assert(can_make_scatter<short>);
static_assert(!can_make_scatter<bool>);
static_assert(!can_make_scatter<double, char>);

TEST_CASE_TEMPLATE("histogram_counts gives the same counts for every type", T, VIZLIB_NUMERIC_TYPES) {
    const std::vector<double> edges{0.5, 1.5, 2.5, 3.5};

    const auto counts = vizlib::histogram_counts(as<T>({1, 2, 2, 3, 3, 3, 7}), edges);

    CHECK(counts == std::vector<std::size_t>{1, 2, 3});
}

TEST_CASE_TEMPLATE("auto_bin_edges matches the double version", T, VIZLIB_NUMERIC_TYPES) {
    CHECK(vizlib::auto_bin_edges(as<T>(xs)) == vizlib::auto_bin_edges(as<double>(xs)));
}

TEST_CASE_TEMPLATE("HistogramPlot accepts any type and rejects empty data", T, VIZLIB_NUMERIC_TYPES) {
    const vizlib::HistogramPlot plot(as<T>({1, 2, 2}), {0, 1.5, 3});
    CHECK(plot.counts() == std::vector<std::size_t>{1, 2});

    CHECK_THROWS_AS(vizlib::HistogramPlot(std::vector<T>{}, {0, 1}), std::invalid_argument);
}

TEST_CASE_TEMPLATE("ScatterPlot<T> has the same bounds as the double version", T, VIZLIB_NUMERIC_TYPES) {
    const vizlib::ScatterPlot<T> plot(as<T>(xs), as<T>(ys));
    const vizlib::ScatterPlot<> reference(as<double>(xs), as<double>(ys));

    CHECK(plot.bounds().x_min == doctest::Approx(reference.bounds().x_min));
    CHECK(plot.bounds().x_max == doctest::Approx(reference.bounds().x_max));
    CHECK(plot.bounds().y_min == doctest::Approx(reference.bounds().y_min));
    CHECK(plot.bounds().y_max == doctest::Approx(reference.bounds().y_max));
}

TEST_CASE_TEMPLATE("scatter draws the same picture for every type", T, VIZLIB_NUMERIC_TYPES) {
    vizlib::Canvas canvas(120, 120, white);
    vizlib::Canvas expected(120, 120, white);

    vizlib::scatter(canvas, vizlib::Rect{10, 10, 100, 100}, as<T>(xs), as<T>(ys));
    vizlib::scatter(expected, vizlib::Rect{10, 10, 100, 100}, as<double>(xs), as<double>(ys));

    CHECK(same_pixels(canvas, expected));
}

TEST_CASE_TEMPLATE("ScatterPlot<T> rejects bad input", T, VIZLIB_NUMERIC_TYPES) {
    CHECK_THROWS_AS(vizlib::ScatterPlot<T>(as<T>({1, 2}), as<T>({1})), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::ScatterPlot<T>(std::vector<T>{}, std::vector<T>{}), std::invalid_argument);
}

TEST_CASE("ScatterPlot can mix types: int on x, double on y") {
    const std::vector<int> years{2020, 2021, 2022, 2023};
    const std::vector<double> prices{1.5, 2.25, 1.75, 3.0};

    const vizlib::ScatterPlot plot(years, prices);   // the compiler works out ScatterPlot<int, double>

    CHECK(plot.bounds().x_min < 2020);
    CHECK(plot.bounds().x_max > 2023);
    CHECK(plot.bounds().y_min < 1.5);
    CHECK(plot.bounds().y_max > 3.0);
}

TEST_CASE("Template plots work through a Plot pointer (runtime polymorphism)") {
    // two different template types and a normal class, all used as Plot
    std::vector<std::unique_ptr<vizlib::Plot>> plots;
    plots.push_back(std::make_unique<vizlib::ScatterPlot<int>>(as<int>(xs), as<int>(ys)));
    plots.push_back(std::make_unique<vizlib::ScatterPlot<short, float>>(as<short>(xs), as<float>(ys)));
    plots.push_back(std::make_unique<vizlib::HistogramPlot>(as<long>(xs), std::vector<double>{0, 5, 10}));

    for (const auto& plot : plots) {
        vizlib::Canvas canvas(60, 60, white);
        vizlib::Axes axes;
        axes.setPlotArea({0, 0, 60, 60});
        const vizlib::DataBounds b = plot->bounds();
        axes.setDataRange(b.x_min, b.x_max, b.y_min, b.y_max);

        plot->draw(canvas, axes);   // virtual call: each plot draws itself

        CHECK_FALSE(same_pixels(canvas, vizlib::Canvas(60, 60, white)));   // something was drawn
    }
}

TEST_CASE_TEMPLATE("JointPlot::with_histograms draws the same for every type", T, VIZLIB_NUMERIC_TYPES) {
    vizlib::Canvas canvas(200, 200, white);
    vizlib::Canvas expected(200, 200, white);

    vizlib::JointPlot::with_histograms(as<T>(xs), as<T>(ys)).draw(canvas, {10, 10, 180, 180});
    vizlib::JointPlot::with_histograms(as<double>(xs), as<double>(ys)).draw(expected, {10, 10, 180, 180});

    CHECK(same_pixels(canvas, expected));
}

TEST_CASE("JointPlot::with_histograms can mix types") {
    vizlib::Canvas canvas(200, 200, white);
    vizlib::Canvas expected(200, 200, white);

    vizlib::JointPlot::with_histograms(as<int>(xs), as<float>(ys)).draw(canvas, {10, 10, 180, 180});
    vizlib::JointPlot::with_histograms(as<double>(xs), as<double>(ys)).draw(expected, {10, 10, 180, 180});

    CHECK(same_pixels(canvas, expected));
}
