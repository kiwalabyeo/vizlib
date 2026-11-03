// Edge-case tests added at the end of the project (3 November 2026).
//
// These inputs were found by testing the library on purpose with bad data:
// missing values (NaN), infinity, all-equal values, one far outlier, huge
// sizes and very small ranges. Each test checks one bug that was fixed. The
// bugs, causes and fixes are listed in docs/bug-report.md. This testing and
// the fixes were done with Claude Code (see reports/ai-use-declaration.md).
#include "doctest.h"

#include "vizlib/canvas.hpp"
#include "vizlib/histogram.hpp"
#include "vizlib/kde.hpp"
#include "vizlib/stats.hpp"
#include "vizlib/text.hpp"
#include "vizlib/ticks.hpp"

#include <cmath>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
const double kNaN = std::numeric_limits<double>::quiet_NaN();
const double kInf = std::numeric_limits<double>::infinity();

bool strictly_increasing(const std::vector<double>& v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (!(v[i] > v[i - 1])) {
            return false;
        }
    }
    return true;
}
}  // namespace

// ---- Bug 1: summaryStatistics and missing values (NaN) ----

TEST_CASE("bug 1: summaryStatistics leaves out NaN wherever it is") {
    // Before the fix the median of these came out as 2, 3 or 4, depending on
    // where the NaN was, because std::sort cannot order NaN.
    for (std::size_t k = 0; k < 5; ++k) {
        std::vector<double> data{5, 4, 3, 2, 1};
        data[k] = kNaN;
        std::vector<double> rest;
        for (std::size_t i = 0; i < 5; ++i) {
            if (i != k) {
                rest.push_back(static_cast<double>(5 - i));
            }
        }
        const vizlib::summaryStatistics with_nan(data);
        const vizlib::summaryStatistics without(rest);
        CHECK(with_nan.median() == doctest::Approx(without.median()));
        CHECK(with_nan.mean() == doctest::Approx(without.mean()));
        CHECK(with_nan.standard_deviation() == doctest::Approx(without.standard_deviation()));
    }
}

TEST_CASE("bug 1: summaryStatistics refuses data that is all NaN, or has infinity") {
    CHECK_THROWS_AS(vizlib::summaryStatistics(std::vector<double>{kNaN, kNaN}),
                    std::invalid_argument);
    CHECK_THROWS_AS(vizlib::summaryStatistics(std::vector<double>{1, kInf}),
                    std::invalid_argument);
}

// ---- Bug 2: one far outlier and Freedman-Diaconis ----

TEST_CASE("bug 2: one far outlier does not make millions of bins") {
    // 1000 values from 0 to 1 and one at 1,000,000. Before the fix this made
    // 10 million bins; with 1e12 the bin count overflowed the int.
    for (double outlier : {1e6, 1e12}) {
        std::vector<double> data;
        for (int i = 0; i < 1000; ++i) {
            data.push_back(i / 1000.0);
        }
        data.push_back(outlier);
        const std::vector<double> edges = vizlib::summaryStatistics(data).histogram_bin_edges();
        CHECK(edges.size() >= 2);
        CHECK(edges.size() <= 1001);
        CHECK(edges.front() == doctest::Approx(0.0));
        CHECK(edges.back() >= outlier * (1 - 1e-9));
        CHECK(strictly_increasing(edges));
    }
}

// ---- Bug 3: data that barely varies ----

TEST_CASE("bug 3: an IQR of 0 falls back to Sturges' rule") {
    // 8 of the 10 values are 1, so the middle half of the data is one value.
    // Before the fix Freedman-Diaconis returned a single edge (no bins).
    const std::vector<double> data{1, 1, 1, 1, 1, 1, 1, 1, 2, 9};
    const std::vector<double> edges = vizlib::summaryStatistics(data).histogram_bin_edges();
    REQUIRE(edges.size() >= 2);
    CHECK(edges.front() == doctest::Approx(1.0));
    CHECK(edges.back() == doctest::Approx(9.0));
    CHECK(strictly_increasing(edges));
    // the edges can be used for a histogram
    CHECK_NOTHROW(vizlib::HistogramPlot(data, edges));
}

TEST_CASE("bug 3: all-equal values give one bin of width 1") {
    // 10 values use Freedman-Diaconis, 50 use Sturges: both used to fail
    for (std::size_t n : {std::size_t{10}, std::size_t{50}}) {
        const std::vector<double> data(n, 2.0);
        const std::vector<double> edges = vizlib::summaryStatistics(data).histogram_bin_edges();
        REQUIRE(edges.size() == 2);
        CHECK(edges[0] == doctest::Approx(1.5));
        CHECK(edges[1] == doctest::Approx(2.5));
        CHECK_NOTHROW(vizlib::HistogramPlot(data, edges));
    }
}

// ---- Bug 4: the KDE and NaN ----

TEST_CASE("bug 4: the KDE leaves out NaN instead of returning NaN everywhere") {
    const vizlib::KdeResult with_nan = vizlib::kde(std::vector<double>{1, kNaN, 3}, 50);
    const vizlib::KdeResult without = vizlib::kde(std::vector<double>{1, 3}, 50);
    CHECK(with_nan.bandwidth == doctest::Approx(without.bandwidth));
    REQUIRE(with_nan.density.size() == without.density.size());
    for (std::size_t i = 0; i < without.density.size(); ++i) {
        CHECK(std::isfinite(with_nan.density[i]));
        CHECK(with_nan.density[i] == doctest::Approx(without.density[i]));
    }
}

TEST_CASE("bug 4: the KDE refuses a bandwidth that is NaN or infinite") {
    const std::vector<double> data{1, 2, 3};
    CHECK_THROWS_AS(vizlib::kde(data, 10, kNaN), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::kde(data, 10, kInf), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::gaussian_density_at(data, 1.0, kNaN), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::kde(std::vector<double>{kNaN}), std::invalid_argument);
}

// ---- Bug 5: a huge Canvas ----

TEST_CASE("bug 5: a huge Canvas is refused instead of overflowing") {
    // 100000 * 100000 does not fit in an int (undefined behaviour before)
    CHECK_THROWS_AS(vizlib::Canvas(100000, 100000, vizlib::Color{}), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::Canvas(20001, 10, vizlib::Color{}), std::invalid_argument);
    CHECK_NOTHROW(vizlib::Canvas(20000, 1, vizlib::Color{}));
}

// ---- Bug 6: infinity in auto_bin_edges ----

TEST_CASE("bug 6: auto_bin_edges refuses infinity") {
    // before the fix this returned the edges NaN, inf, inf, inf
    CHECK_THROWS_AS(vizlib::auto_bin_edges(std::vector<double>{1, kInf, 3}),
                    std::invalid_argument);
    CHECK_THROWS_AS(vizlib::auto_bin_edges(std::vector<double>{-kInf, 1, 3}),
                    std::invalid_argument);
}

// ---- Smaller fixes: ticks, tick labels, font size ----

TEST_CASE("ticks: a short log range still gets at least two ticks") {
    // 2 to 5 holds no power of 10, 20 to 50 only one: both used to give < 2
    for (const auto& [lo, hi] : {std::pair{2.0, 5.0}, std::pair{20.0, 50.0}}) {
        const std::vector<double> ticks = vizlib::generateLogTicks(lo, hi);
        CHECK(ticks.size() >= 2);
        for (double t : ticks) {
            CHECK(t >= lo);
            CHECK(t <= hi);
        }
    }
    // a normal log range is unchanged
    CHECK(vizlib::generateLogTicks(1, 1000) == std::vector<double>{1, 10, 100, 1000});
}

TEST_CASE("ticks: a log range over many powers of 10 keeps about 10 ticks") {
    const std::vector<double> ticks = vizlib::generateLogTicks(1e-300, 1e300);
    CHECK(ticks.size() >= 2);
    CHECK(ticks.size() <= 11);
    CHECK(ticks.front() == doctest::Approx(1e-300));
}

TEST_CASE("ticks: a range too big to subtract is refused, not empty") {
    CHECK_THROWS_AS(vizlib::generateTicks(-1e308, 1e308), std::invalid_argument);
}

TEST_CASE("ticks: no rounding drift, ticks are exact multiples of the step") {
    const std::vector<double> ticks = vizlib::generateTicks(0.0, 1.0, 11);
    REQUIRE(ticks.size() == 11);
    CHECK(ticks[3] == 0.30000000000000004);  // 3 * 0.1, not 0.1 + 0.1 + 0.1
    CHECK(ticks.back() == doctest::Approx(1.0));
}

TEST_CASE("tick labels: neighbouring ticks never print the same") {
    // with setprecision(4) these all printed as "1.234e+04" or "1e+15"
    const std::vector<std::pair<double, double>> ranges{
        {12345, 12355}, {1e15, 1e15 + 10}, {0.001, 0.0011}, {1.2345, 1.2355}, {0, 40}};
    for (const auto& [lo, hi] : ranges) {
        const std::vector<double> ticks = vizlib::generateTicks(lo, hi);
        std::set<std::string> labels;
        for (double t : ticks) {
            labels.insert(vizlib::formatTickLabel(t, ticks));
        }
        CHECK(labels.size() == ticks.size());
    }
    const std::vector<double> ticks{0, 0.5, 1};
    CHECK(vizlib::formatTickLabel(0.5, ticks) == "0.5");
    CHECK(vizlib::formatTickLabel(1, ticks) == "1.0");
    const std::vector<double> whole{0, 20, 40};
    CHECK(vizlib::formatTickLabel(20, whole) == "20");
    CHECK(vizlib::formatTickLabel(100, {1, 10, 100}, true) == "100");
}

TEST_CASE("text: a font size of NaN or infinity is refused") {
    const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
    vizlib::Canvas canvas(50, 50, vizlib::Color{255, 255, 255, 255});
    CHECK_THROWS_AS(text.measure_text("Hi", kNaN), std::invalid_argument);
    CHECK_THROWS_AS(text.draw_text(canvas, "Hi", {5, 20}, kNaN, vizlib::Color{}),
                    std::invalid_argument);
    CHECK_THROWS_AS(text.draw_text(canvas, "Hi", {5, 20}, kInf, vizlib::Color{}),
                    std::invalid_argument);
}
