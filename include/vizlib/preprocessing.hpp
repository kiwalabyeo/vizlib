#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "vizlib/dataframe.hpp"
#include "vizlib/numeric.hpp"

namespace vizlib {

// --- Missing values --------------------------------------------------------

// Remove every row that has a missing cell in any column.
DataFrame drop_missing(const DataFrame& df);

// Name used by the first version of the library. Same as drop_missing(df).
DataFrame handle_missing_values(const DataFrame& df);

// Remove every row that has a missing cell in one of the listed columns.
// Throws std::invalid_argument if a listed column does not exist.
DataFrame drop_missing(const DataFrame& df, const std::vector<std::string>& columns);

// Replace missing cells of a numeric column with `value`.
// Throws std::invalid_argument if the column is missing or is a text column.
DataFrame fill_missing(const DataFrame& df, const std::string& column, double value);

// Replace missing cells of a text column with `value`.
// Throws std::invalid_argument if the column is missing or is numeric.
DataFrame fill_missing(const DataFrame& df, const std::string& column, const std::string& value);

// Same, with a value of any other numeric type (int, long, float, ...). The
// value is converted to double; throws std::invalid_argument if a double
// cannot hold it exactly. fill_missing(df, "age", 30) works.
template <Numeric T>
DataFrame fill_missing(const DataFrame& df, const std::string& column, T value) {
    return fill_missing(df, column, to_double_exact(value));
}

// Replace missing cells of a numeric column with the mean / median of its
// non-missing cells. Throws std::invalid_argument if the column is text or has
// no non-missing cells.
DataFrame fill_missing_mean(const DataFrame& df, const std::string& column);
DataFrame fill_missing_median(const DataFrame& df, const std::string& column);

// The values that are not NaN, in the same order. A missing cell in a numeric
// column is stored as NaN, so this is the one place the library leaves missing
// values out of a plain list of numbers. The ECDF, rug plot, joint plot KDE
// margins and the statistics all call it.
std::vector<double> drop_nan(const std::vector<double>& values);

// --- Types -----------------------------------------------------------------

// Convert a text column to numbers. Cells that are not numbers throw
// std::invalid_argument, unless coerce is true, in which case they become
// missing (NaN). A column that is already numeric is returned unchanged.
DataFrame to_numeric(const DataFrame& df, const std::string& column, bool coerce = false);

// Parse one string as a plain decimal number. Returns false if it is not one.
// (Shared with the CSV reader so both agree on what a number is.)
bool parse_number(const std::string& s, double& out);

// Same, into any numeric type. Returns false if `s` is not a number or the
// number does not fit T exactly (e.g. "2.5" or "3000000000" into an int).
template <Numeric T>
bool parse_number(const std::string& s, T& out) {
    double d = 0;
    if (!parse_number(s, d) || !is_representable<T>(d)) return false;
    out = static_cast<T>(d);
    return true;
}

// --- Rows and columns ------------------------------------------------------

// Keep the rows for which predicate(df, row_index) is true.
DataFrame filter_rows(const DataFrame& df,
                      const std::function<bool(const DataFrame&, std::size_t)>& predicate);

// Keep rows where predicate(value) is true for a numeric column. Missing
// cells never pass. Throws std::invalid_argument for text or unknown columns.
DataFrame filter_numeric(const DataFrame& df, const std::string& column,
                         const std::function<bool(double)>& predicate);

// Same, but the predicate takes any numeric type T, chosen explicitly:
//   filter_numeric<int>(df, "size", [](int s) { return s >= 3; });
// For an integer T every non-missing value of the column must be a whole number
// in range, otherwise std::invalid_argument names the row.
template <Numeric T>
DataFrame filter_numeric(const DataFrame& df, const std::string& column,
                         const std::function<bool(T)>& predicate) {
    const Column& c = df[column];
    const std::vector<double>& v = c.values();  // throws for a text column
    std::vector<std::size_t> keep;
    for (std::size_t r = 0; r < v.size(); ++r) {
        if (c.is_missing(r)) continue;
        if (!is_representable<T>(v[r]))
            throw std::invalid_argument("Column '" + column + "', row " + std::to_string(r) +
                                        ": value cannot be converted to the requested type");
        if (predicate(static_cast<T>(v[r]))) keep.push_back(r);
    }
    return df.take_rows(keep);
}

// Keep only the listed columns, in the order listed.
// Throws std::invalid_argument for an unknown or repeated name.
DataFrame select_columns(const DataFrame& df, const std::vector<std::string>& columns);

// Remove the listed columns. Throws std::invalid_argument for an unknown name.
DataFrame drop_columns(const DataFrame& df, const std::vector<std::string>& columns);

}  // namespace vizlib
