#include "vizlib/preprocessing.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <locale>
#include <set>
#include <sstream>
#include <stdexcept>

namespace vizlib {

namespace {

const Column& numeric_column(const DataFrame& df, const std::string& name) {
    const Column& c = df[name];  // throws if the column does not exist
    if (!c.is_numeric())
        throw std::invalid_argument("Column '" + name + "' is a text column, expected numeric");
    return c;
}

// Rebuild df with one column swapped for a new one.
DataFrame with_replaced(const DataFrame& df, Column column) {
    DataFrame out = df;
    out.replace_column(std::move(column));
    return out;
}

std::vector<double> non_missing(const Column& c) {
    return drop_nan(c.values());   // a missing numeric cell is NaN
}

}  // namespace

std::vector<double> drop_nan(const std::vector<double>& values) {
    std::vector<double> out;
    out.reserve(values.size());
    for (double v : values) {
        if (!std::isnan(v)) out.push_back(v);
    }
    return out;
}

bool parse_number(const std::string& s, double& out) {
    // Only plain decimal notation: this rejects "inf", "nan" and hex like 0x1A.
    const std::size_t a = s.find_first_not_of(" \t");
    if (a == std::string::npos) return false;
    const std::size_t b = s.find_last_not_of(" \t");
    const std::string t = s.substr(a, b - a + 1);
    if (t.find_first_not_of("0123456789+-.eE") != std::string::npos) return false;
    if (t.find_first_of("0123456789") == std::string::npos) return false;

    std::istringstream in(t);
    in.imbue(std::locale::classic());  // always '.' as the decimal point
    double v = 0;
    in >> v;
    if (in.fail() || in.peek() != std::char_traits<char>::eof()) return false;
    if (!std::isfinite(v)) return false;
    out = v;
    return true;
}

DataFrame drop_missing(const DataFrame& df) {
    return drop_missing(df, df.column_names());
}

DataFrame handle_missing_values(const DataFrame& df) {
    return drop_missing(df);
}

DataFrame drop_missing(const DataFrame& df, const std::vector<std::string>& columns) {
    std::vector<const Column*> check;
    for (const auto& name : columns) check.push_back(&df[name]);  // validates names
    std::vector<std::size_t> keep;
    for (std::size_t r = 0; r < df.n_rows(); ++r) {
        bool ok = true;
        for (const Column* c : check)
            if (c->is_missing(r)) {
                ok = false;
                break;
            }
        if (ok) keep.push_back(r);
    }
    return df.take_rows(keep);
}

DataFrame fill_missing(const DataFrame& df, const std::string& column, double value) {
    const Column& c = numeric_column(df, column);
    std::vector<double> v = c.values();
    for (double& x : v)
        if (std::isnan(x)) x = value;
    return with_replaced(df, Column(column, std::move(v)));
}

DataFrame fill_missing(const DataFrame& df, const std::string& column, const std::string& value) {
    const Column& c = df[column];
    if (c.is_numeric())
        throw std::invalid_argument("Column '" + column + "' is numeric; fill it with a number");
    std::vector<std::string> v = c.text();
    for (std::size_t i = 0; i < v.size(); ++i)
        if (c.is_missing(i)) v[i] = value;
    return with_replaced(df, Column(column, std::move(v)));  // nothing missing any more
}

DataFrame fill_missing_mean(const DataFrame& df, const std::string& column) {
    const std::vector<double> v = non_missing(numeric_column(df, column));
    if (v.empty())
        throw std::invalid_argument("Column '" + column + "' has no values to take a mean of");
    double sum = 0;
    for (double x : v) sum += x;
    return fill_missing(df, column, sum / static_cast<double>(v.size()));
}

DataFrame fill_missing_median(const DataFrame& df, const std::string& column) {
    std::vector<double> v = non_missing(numeric_column(df, column));
    if (v.empty())
        throw std::invalid_argument("Column '" + column + "' has no values to take a median of");
    std::sort(v.begin(), v.end());
    const std::size_t n = v.size();
    const double med = (n % 2 == 1) ? v[n / 2] : (v[n / 2 - 1] + v[n / 2]) / 2.0;
    return fill_missing(df, column, med);
}

DataFrame to_numeric(const DataFrame& df, const std::string& column, bool coerce) {
    const Column& c = df[column];
    if (c.is_numeric()) return df;
    std::vector<double> out(c.size(), std::numeric_limits<double>::quiet_NaN());
    for (std::size_t i = 0; i < c.size(); ++i) {
        if (c.is_missing(i)) continue;
        double v;
        if (parse_number(c.text()[i], v)) {
            out[i] = v;
        } else if (!coerce) {
            throw std::invalid_argument("Column '" + column + "', row " + std::to_string(i) +
                                        ": '" + c.text()[i] + "' is not a number");
        }
    }
    return with_replaced(df, Column(column, std::move(out)));
}

DataFrame filter_rows(const DataFrame& df,
                      const std::function<bool(const DataFrame&, std::size_t)>& predicate) {
    std::vector<std::size_t> keep;
    for (std::size_t r = 0; r < df.n_rows(); ++r)
        if (predicate(df, r)) keep.push_back(r);
    return df.take_rows(keep);
}

DataFrame filter_numeric(const DataFrame& df, const std::string& column,
                         const std::function<bool(double)>& predicate) {
    const Column& c = numeric_column(df, column);
    std::vector<std::size_t> keep;
    for (std::size_t r = 0; r < c.size(); ++r)
        if (!c.is_missing(r) && predicate(c.values()[r])) keep.push_back(r);
    return df.take_rows(keep);
}

DataFrame select_columns(const DataFrame& df, const std::vector<std::string>& columns) {
    DataFrame out;
    std::set<std::string> seen;
    for (const auto& name : columns) {
        if (!seen.insert(name).second)
            throw std::invalid_argument("Column '" + name + "' listed twice");
        out.add_column(df[name]);
    }
    return out;
}

DataFrame drop_columns(const DataFrame& df, const std::vector<std::string>& columns) {
    for (const auto& name : columns) df[name];  // validates names
    const std::set<std::string> drop(columns.begin(), columns.end());
    DataFrame out;
    for (const auto& name : df.column_names())
        if (!drop.count(name)) out.add_column(df[name]);
    return out;
}

}  // namespace vizlib
