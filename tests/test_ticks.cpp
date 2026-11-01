// Converted from a main() program with assert() into doctest checks.
// (assert does nothing in a Release build, which is what CI uses.)
#include "doctest.h"

#include "vizlib/ticks.hpp"

#include <stdexcept>

TEST_CASE("generateTicks gives nice ticks from min to max") {
    const auto ticks = vizlib::generateTicks(0, 20, 5);

    REQUIRE_FALSE(ticks.empty());
    CHECK(ticks.front() == 0);
    CHECK(ticks.back() == 20);
    CHECK(ticks.size() == 5);

    CHECK_FALSE(vizlib::generateTicks(0, 1, 5).empty());
}

TEST_CASE("generateTicks rejects an empty range") {
    CHECK_THROWS_AS(vizlib::generateTicks(10, 10), std::invalid_argument);
}
