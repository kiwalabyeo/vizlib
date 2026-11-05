#include "doctest.h"
#include "numeric_types.hpp"

#include "vizlib/kde.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace {
// area under the curve with the trapezium rule
double area(const vizlib::KdeResult& r) {
    double total = 0;
    for (std::size_t i = 1; i < r.grid.size(); ++i) {
        total += (r.grid[i] - r.grid[i - 1]) * (r.density[i] + r.density[i - 1]) / 2.0;
    }
    return total;
}
}  // namespace

TEST_CASE("kde: the area under the curve is 1") {
    const vizlib::KdeResult r = vizlib::kde(std::vector<double>{1, 2, 2, 3, 7, 8, 8.5});
    CHECK(area(r) == doctest::Approx(1.0).epsilon(0.01));
}

TEST_CASE("kde: grid has the asked size and runs 3 bandwidths past the data") {
    const vizlib::KdeResult r = vizlib::kde(std::vector<double>{1, 5}, 50, 0.5);

    REQUIRE(r.grid.size() == 50);
    REQUIRE(r.density.size() == 50);
    CHECK(r.bandwidth == 0.5);
    CHECK(r.grid.front() == doctest::Approx(1 - 3 * 0.5));
    CHECK(r.grid.back() == doctest::Approx(5 + 3 * 0.5));
}

TEST_CASE("kde: symmetric data peaks in the middle") {
    const vizlib::KdeResult r = vizlib::kde(std::vector<double>{1, 2, 3}, 201);
    const auto peak = std::max_element(r.density.begin(), r.density.end()) - r.density.begin();
    CHECK(r.grid[peak] == doctest::Approx(2.0).epsilon(0.02));
}

TEST_CASE("silverman_bandwidth follows h = 0.9 * spread * n^(-1/5)") {
    // {1..5}: standard deviation sqrt(2), IQR/1.34 = 2/1.34 is bigger, so the spread is sqrt(2)
    CHECK(vizlib::silverman_bandwidth({1, 2, 3, 4, 5}) ==
          doctest::Approx(0.9 * std::sqrt(2.0) * std::pow(5.0, -0.2)));

    // all values equal: no spread, so 1 is used and h stays above 0
    CHECK(vizlib::silverman_bandwidth({4, 4, 4, 4}) == doctest::Approx(0.9 * std::pow(4.0, -0.2)));
}

TEST_CASE("kde rejects bad input") {
    CHECK_THROWS_AS(vizlib::kde(std::vector<double>{}), std::invalid_argument);
    CHECK_THROWS_AS(vizlib::kde(std::vector<double>{1, 2}, 1), std::invalid_argument);       // grid too small
    CHECK_THROWS_AS(vizlib::kde(std::vector<double>{1, 2}, 10, -1), std::invalid_argument);  // negative bandwidth
}

TEST_CASE_TEMPLATE("kde gives the same curve for every number type", T, VIZLIB_NUMERIC_TYPES) {
    const std::vector<int> base{1, 2, 2, 3, 5, 8};
    const vizlib::KdeResult r = vizlib::kde(std::vector<T>(base.begin(), base.end()), 40);
    const vizlib::KdeResult expected = vizlib::kde(std::vector<double>(base.begin(), base.end()), 40);

    CHECK(r.bandwidth == doctest::Approx(expected.bandwidth));
    for (std::size_t i = 0; i < r.density.size(); ++i) {
        CHECK(r.density[i] == doctest::Approx(expected.density[i]));
    }
}
