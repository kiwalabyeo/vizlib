#include "doctest.h"
#include "numeric_types.hpp"

#include "vizlib/concepts.hpp"

#include <cstdint>
#include <string>
#include <vector>

// These are checked when the test file compiles, before any test runs.
static_assert(vizlib::Numeric<int>);
static_assert(vizlib::Numeric<short>);
static_assert(vizlib::Numeric<long long>);
static_assert(vizlib::Numeric<unsigned int>);
static_assert(vizlib::Numeric<std::int8_t>);
static_assert(vizlib::Numeric<float>);
static_assert(vizlib::Numeric<double>);
static_assert(vizlib::Numeric<const double>);

static_assert(!vizlib::Numeric<bool>);
static_assert(!vizlib::Numeric<char>);
static_assert(!vizlib::Numeric<std::string>);
static_assert(!vizlib::Numeric<double*>);

TEST_CASE_TEMPLATE("to_doubles keeps every value and the order", T, VIZLIB_NUMERIC_TYPES) {
    const std::vector<T> values{T(3), T(1), T(2)};

    const std::vector<double> result = vizlib::to_doubles(values);

    REQUIRE(result.size() == 3);
    CHECK(result[0] == 3.0);
    CHECK(result[1] == 1.0);
    CHECK(result[2] == 2.0);
}

TEST_CASE_TEMPLATE("to_doubles of an empty vector is empty", T, VIZLIB_NUMERIC_TYPES) {
    CHECK(vizlib::to_doubles(std::vector<T>{}).empty());
}
