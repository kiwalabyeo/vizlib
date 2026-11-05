// typed_column.hpp - NumericColumn<T>: a column held as a chosen numeric type
// (Member 1)
//
// Column stores numbers as double. NumericColumn<T> is a class template that
// copies one numeric Column into a std::vector<T>, for any numeric T (int,
// long, short, float, double, unsigned, ...). Use it when an algorithm wants
// real integers, e.g. counts for a histogram:
//
//     NumericColumn<int> sizes(df["size"]);
//     for (int s : sizes) { ... }
//
// The conversion rules are those of Column::as<T>(): nothing is rounded or
// wrapped silently; a missing cell, a fractional value or a value out of range
// throws std::invalid_argument when T is an integer type.
#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

#include "vizlib/dataframe.hpp"
#include "vizlib/numeric.hpp"

namespace vizlib {

VIZLIB_NUMERIC_TEMPLATE(T)
class NumericColumn {
public:
    using value_type = T;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Copy a numeric column, converting every value to T.
    // Throws std::invalid_argument for a text column or a value that does not fit.
    explicit NumericColumn(const Column& column)
        : name_(column.name()), data_(column.as<T>()) {}

    // Build directly from values.
    NumericColumn(std::string name, std::vector<T> values)
        : name_(std::move(name)), data_(std::move(values)) {}

    const std::string& name() const { return name_; }
    std::size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }

    // Unchecked access, like std::vector::operator[].
    const T& operator[](std::size_t i) const { return data_[i]; }
    // Checked access. Throws std::out_of_range.
    const T& at(std::size_t i) const {
        if (i >= data_.size()) throw std::out_of_range("NumericColumn '" + name_ + "': index out of range");
        return data_.at(i);
    }

    const std::vector<T>& data() const { return data_; }
    const_iterator begin() const { return data_.begin(); }
    const_iterator end() const { return data_.end(); }

    // Sum of all values, accumulated in double so small integer types cannot
    // overflow. NaN values (floating types only) make the sum NaN.
    double sum() const {
        double s = 0;
        for (const T& x : data_) s += static_cast<double>(x);
        return s;
    }

    // Convert back to a Column (stored as double). Throws std::invalid_argument
    // if a value cannot be held exactly by a double.
    Column to_column() const { return Column(name_, data_); }

private:
    std::string name_;
    std::vector<T> data_;
};

}  // namespace vizlib
