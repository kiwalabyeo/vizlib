// dataframe.hpp - Column and DataFrame (Member 1)
//
// A DataFrame is a table of named columns. Each Column is either numeric
// (stored as double, with NaN meaning "missing") or text (stored as strings
// with a separate missing flag per cell).
//
// Errors (project rule): std::invalid_argument for bad input,
// std::runtime_error for things like a missing file.
#pragma once

#include <cmath>
#include <cstddef>
#include <string>
#include <variant>
#include <vector>

namespace vizlib {

// One cell as the first version of the library stored it: a number or a string.
// Kept so code written against that version (get_column, row_count, ...) still
// compiles. New code should use Column and df["name"].
using CellValue = std::variant<double, std::string>;

class Column {
public:
    enum class Type { Numeric, Text };

    // Numeric column. NaN entries are treated as missing.
    Column(std::string name, std::vector<double> values);

    // Text column. `missing` is either empty (nothing missing) or the same
    // length as `values`, where a non-zero entry marks that cell as missing.
    // Throws std::invalid_argument if the lengths differ.
    Column(std::string name, std::vector<std::string> values,
           std::vector<char> missing = {});

    const std::string& name() const { return name_; }
    Type type() const { return type_; }
    bool is_numeric() const { return type_ == Type::Numeric; }
    std::size_t size() const;

    // True if cell `i` is missing. Throws std::out_of_range if i >= size().
    bool is_missing(std::size_t i) const;
    std::size_t count_missing() const;

    // Values as numbers. Throws std::invalid_argument on a text column
    // (convert it first with to_numeric() from preprocessing.hpp).
    // Missing cells are NaN.
    const std::vector<double>& values() const;

    // A Column converts to its numbers, so a Column can be passed straight to
    // functions that take std::vector<double>, e.g. ecdf_plot(ax, df["tip"]).
    // Throws std::invalid_argument on a text column.
    operator const std::vector<double>&() const { return values(); }

    // Text of a text column. Throws std::invalid_argument on a numeric column.
    const std::vector<std::string>& text() const;

    // Cell `i` as a string (numbers are formatted, missing cells give "").
    std::string as_string(std::size_t i) const;

    // A new column with the same name holding only the given rows, in the
    // order given. Throws std::out_of_range for a bad index.
    Column take(const std::vector<std::size_t>& rows) const;

    // A copy with a different name.
    Column renamed(std::string new_name) const;

private:
    std::string name_;
    Type type_;
    std::vector<double> numbers_;
    std::vector<std::string> strings_;
    std::vector<char> missing_;  // text columns only
};

class DataFrame {
public:
    DataFrame() = default;

    // Add a column at the right-hand end. Throws std::invalid_argument if the
    // name already exists or the length differs from the existing columns.
    void add_column(Column column);

    std::size_t n_rows() const;
    std::size_t n_cols() const { return columns_.size(); }
    bool empty() const { return n_rows() == 0 || n_cols() == 0; }

    bool has_column(const std::string& name) const;
    std::vector<std::string> column_names() const;

    // Get a column by name. Throws std::invalid_argument if it does not exist.
    const Column& operator[](const std::string& name) const;
    const Column& column(const std::string& name) const { return (*this)[name]; }
    // Get a column by position. Throws std::out_of_range.
    const Column& column(std::size_t index) const;

    // Replace an existing column (same length required, same or new type).
    // Throws std::invalid_argument if the name does not exist.
    void replace_column(Column column);

    // A new DataFrame with only the given rows, in the order given.
    DataFrame take_rows(const std::vector<std::size_t>& rows) const;

    // --- Compatibility with the first version of the library ---------------
    // Same as n_rows() / n_cols().
    std::size_t row_count() const { return n_rows(); }
    std::size_t column_count() const { return n_cols(); }

    // A copy of a column as a vector of numbers/strings. Numeric columns give
    // doubles (missing = NaN); text columns give strings (missing = "").
    // Throws std::invalid_argument if the column does not exist.
    std::vector<CellValue> get_column(const std::string& name) const;

    // Add a column from numbers/strings. If every cell is a double the column is
    // numeric; otherwise it is a text column (doubles are written as text and
    // empty strings count as missing). Throws std::invalid_argument on a
    // duplicate name or a length that differs from the other columns.
    void add_column(const std::string& name, const std::vector<CellValue>& data);

    // First n rows rendered as aligned text, for debugging and examples.
    std::string to_string(std::size_t max_rows = 5) const;

private:
    std::size_t index_of(const std::string& name) const;  // npos if absent
    std::vector<Column> columns_;
};

}  // namespace vizlib
