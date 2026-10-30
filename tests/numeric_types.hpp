#pragma once

// The number types every templated module is tested with.
// Use it with doctest's TEST_CASE_TEMPLATE so one test runs once per type:
//
//   TEST_CASE_TEMPLATE("mean works for every numeric type", T, VIZLIB_NUMERIC_TYPES) {
//       std::vector<T> data{1, 2, 3};
//       CHECK(vizlib::mean(data) == doctest::Approx(2.0));
//   }
#define VIZLIB_NUMERIC_TYPES short, int, long, long long, unsigned int, float, double
