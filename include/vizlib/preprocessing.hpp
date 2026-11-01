#pragma once

#include <functional>
#include <string>
#include <vector>

#include "vizlib/dataframe.hpp"

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

// Replace missing cells of a numeric column with the mean / median of its
// non-missing cells. Throws std::invalid_argument if the column is text or has
// no non-missing cells.
DataFrame fill_missing_mean(const DataFrame& df, const std::string& column);
DataFrame fill_missing_median(const DataFrame& df, const std::string& column);

// --- Types -----------------------------------------------------------------

// Convert a text column to numbers. Cells that are not numbers throw
// std::invalid_argument, unless coerce is true, in which case they become
// missing (NaN). A column that is already numeric is returned unchanged.
DataFrame to_numeric(const DataFrame& df, const std::string& column, bool coerce = false);

// Parse one string as a plain decimal number. Returns false if it is not one.
// (Shared with the CSV reader so both agree on what a number is.)
bool parse_number(const std::string& s, double& out);

// --- Rows and columns ------------------------------------------------------

// Keep the rows for which predicate(df, row_index) is true.
DataFrame filter_rows(const DataFrame& df,
                      const std::function<bool(const DataFrame&, std::size_t)>& predicate);

// Keep rows where predicate(value) is true for a numeric column. Missing
// cells never pass. Throws std::invalid_argument for text or unknown columns.
DataFrame filter_numeric(const DataFrame& df, const std::string& column,
                         const std::function<bool(double)>& predicate);

// Keep only the listed columns, in the order listed.
// Throws std::invalid_argument for an unknown or repeated name.
DataFrame select_columns(const DataFrame& df, const std::vector<std::string>& columns);

// Remove the listed columns. Throws std::invalid_argument for an unknown name.
DataFrame drop_columns(const DataFrame& df, const std::vector<std::string>& columns);

}  // namespace vizlib
