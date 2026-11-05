// numeric.hpp - which types count as "numeric", and safe conversions (Member 1)
//
// vizlib stores numbers as double inside a Column. The templates in the data
// module let the caller read and write them as ANY numeric type (int, long,
// short, float, double, unsigned, ...). This header decides which types are
// allowed and converts between them without silently losing information.
//
// Concepts: when the code is compiled as C++20 the constraint is the real
// concept `vizlib::Numeric`. The project is built as C++17, which has no
// concepts, so there the same rule is enforced with std::enable_if. Both are
// written once, in the macro VIZLIB_NUMERIC_TEMPLATE(T), so every template in
// the module reads the same way:
//
//     VIZLIB_NUMERIC_TEMPLATE(T)
//     std::vector<T> as() const;
//
// means "template <Numeric T>" in C++20 and the enable_if form in C++17.
#pragma once

#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace vizlib {

// A numeric type is any arithmetic type except bool.
template <typename T>
inline constexpr bool is_numeric_type_v =
    std::is_arithmetic_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>;

#if defined(__cpp_concepts) && __cpp_concepts >= 201907L
#define VIZLIB_HAS_CONCEPTS 1
template <typename T>
concept Numeric = is_numeric_type_v<T>;
#define VIZLIB_NUMERIC_TEMPLATE(T) template <::vizlib::Numeric T>
#else
#define VIZLIB_HAS_CONCEPTS 0
#define VIZLIB_NUMERIC_TEMPLATE(T) \
    template <typename T, typename = std::enable_if_t<::vizlib::is_numeric_type_v<T>>>
#endif

// True if the double `x` can be stored in a T without changing its value.
//  - Integer types: x must be finite, have no fractional part and be in range.
//    NaN (a missing value) is NOT representable.
//  - Floating types: NaN and infinity are kept; finite values must be in range.
template <typename T>
bool is_representable(double x) {
    static_assert(is_numeric_type_v<T>, "T must be a numeric type");
    if constexpr (std::is_floating_point_v<T>) {
        if (std::isnan(x) || std::isinf(x)) return true;
        return std::fabs(x) <= static_cast<double>(std::numeric_limits<T>::max());
    } else {
        if (!std::isfinite(x) || x != std::trunc(x)) return false;
        // 2^digits is exactly one more than the largest value, and is exactly
        // representable as a double, so the comparison is exact for every type.
        const double upper = std::ldexp(1.0, std::numeric_limits<T>::digits);
        const double lower = std::is_signed_v<T> ? -upper : 0.0;
        return x >= lower && x < upper;
    }
}

// Convert a value of any numeric type to double. Throws std::invalid_argument
// if the double would not hold the value exactly (for example a 64-bit integer
// above 2^53).
template <typename T>
double to_double_exact(T x) {
    static_assert(is_numeric_type_v<T>, "T must be a numeric type");
    const double d = static_cast<double>(x);
    bool exact = true;
    if constexpr (std::is_floating_point_v<T>) {
        exact = std::isnan(x) || d == x;
    } else {
        exact = is_representable<T>(d) && static_cast<T>(d) == x;
    }
    if (!exact)
        throw std::invalid_argument("The value " + std::to_string(x) +
                                    " cannot be stored exactly as a double");
    return d;
}

}  // namespace vizlib
