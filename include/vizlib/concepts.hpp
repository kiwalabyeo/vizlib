#pragma once

#include <concepts>
#include <type_traits>
#include <vector>

// Shared concepts for vizlib templates.
// Use these instead of writing your own, so every module accepts the same types.

namespace vizlib {

// Any built-in number type: short, int, long, long long, unsigned, float, double, ...
// bool and char also count as integers in C++, but they are never data you would plot,
// so they are left out. Passing them gives a clear compile error instead of a strange plot.
template <typename T>
concept Numeric =
    (std::integral<T> || std::floating_point<T>) &&
    !std::same_as<std::remove_cv_t<T>, bool> &&
    !std::same_as<std::remove_cv_t<T>, char>;

// Copies any numeric vector into a vector<double>.
// Most maths (mean, quantiles, pixel positions) needs doubles anyway, so a template
// function can convert its input once at the start and keep the rest of its code simple.
template <Numeric T>
std::vector<double> to_doubles(const std::vector<T>& values) {
    return std::vector<double>(values.begin(), values.end());
}

}  // namespace vizlib
