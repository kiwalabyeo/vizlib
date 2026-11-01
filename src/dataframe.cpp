#include "vizlib/dataframe.hpp"

#include <algorithm>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace vizlib {

namespace {
constexpr std::size_t npos = static_cast<std::size_t>(-1);

std::string format_number(double v) {
    std::ostringstream os;
    os.imbue(std::locale::classic());
    os.precision(10);
    os << v;
    return os.str();
}
}  // namespace

// ---------------------------------------------------------------- Column --

Column::Column(std::string name, std::vector<double> values)
    : name_(std::move(name)), type_(Type::Numeric), numbers_(std::move(values)) {}

Column::Column(std::string name, std::vector<std::string> values, std::vector<char> missing)
    : name_(std::move(name)),
      type_(Type::Text),
      strings_(std::move(values)),
      missing_(std::move(missing)) {
    if (missing_.empty()) {
        missing_.assign(strings_.size(), 0);
    } else if (missing_.size() != strings_.size()) {
        throw std::invalid_argument("Column '" + name_ +
                                    "': missing flags must match the number of values");
    }
}

std::size_t Column::size() const {
    return is_numeric() ? numbers_.size() : strings_.size();
}

bool Column::is_missing(std::size_t i) const {
    if (i >= size()) throw std::out_of_range("Column '" + name_ + "': row index out of range");
    return is_numeric() ? std::isnan(numbers_[i]) : missing_[i] != 0;
}

std::size_t Column::count_missing() const {
    std::size_t n = 0;
    for (std::size_t i = 0; i < size(); ++i)
        if (is_missing(i)) ++n;
    return n;
}

const std::vector<double>& Column::values() const {
    if (!is_numeric())
        throw std::invalid_argument("Column '" + name_ +
                                    "' is a text column; convert it with to_numeric() first");
    return numbers_;
}

const std::vector<std::string>& Column::text() const {
    if (is_numeric())
        throw std::invalid_argument("Column '" + name_ + "' is numeric, not text");
    return strings_;
}

std::string Column::as_string(std::size_t i) const {
    if (is_missing(i)) return "";
    return is_numeric() ? format_number(numbers_[i]) : strings_[i];
}

Column Column::take(const std::vector<std::size_t>& rows) const {
    for (std::size_t r : rows)
        if (r >= size()) throw std::out_of_range("Column '" + name_ + "': row index out of range");
    if (is_numeric()) {
        std::vector<double> out;
        out.reserve(rows.size());
        for (std::size_t r : rows) out.push_back(numbers_[r]);
        return Column(name_, std::move(out));
    }
    std::vector<std::string> out;
    std::vector<char> flags;
    out.reserve(rows.size());
    flags.reserve(rows.size());
    for (std::size_t r : rows) {
        out.push_back(strings_[r]);
        flags.push_back(missing_[r]);
    }
    return Column(name_, std::move(out), std::move(flags));
}

Column Column::renamed(std::string new_name) const {
    Column copy = *this;
    copy.name_ = std::move(new_name);
    return copy;
}

// ------------------------------------------------------------- DataFrame --

std::size_t DataFrame::n_rows() const {
    return columns_.empty() ? 0 : columns_.front().size();
}

std::size_t DataFrame::index_of(const std::string& name) const {
    for (std::size_t i = 0; i < columns_.size(); ++i)
        if (columns_[i].name() == name) return i;
    return npos;
}

void DataFrame::add_column(Column column) {
    if (index_of(column.name()) != npos)
        throw std::invalid_argument("DataFrame already has a column named '" + column.name() + "'");
    if (!columns_.empty() && column.size() != n_rows())
        throw std::invalid_argument("Column '" + column.name() + "' has " +
                                    std::to_string(column.size()) + " rows but the DataFrame has " +
                                    std::to_string(n_rows()));
    columns_.push_back(std::move(column));
}

bool DataFrame::has_column(const std::string& name) const {
    return index_of(name) != npos;
}

std::vector<std::string> DataFrame::column_names() const {
    std::vector<std::string> names;
    names.reserve(columns_.size());
    for (const auto& c : columns_) names.push_back(c.name());
    return names;
}

const Column& DataFrame::operator[](const std::string& name) const {
    std::size_t i = index_of(name);
    if (i == npos) throw std::invalid_argument("No column named '" + name + "'");
    return columns_[i];
}

const Column& DataFrame::column(std::size_t index) const {
    if (index >= columns_.size()) throw std::out_of_range("Column index out of range");
    return columns_[index];
}

void DataFrame::replace_column(Column column) {
    std::size_t i = index_of(column.name());
    if (i == npos) throw std::invalid_argument("No column named '" + column.name() + "'");
    if (column.size() != n_rows())
        throw std::invalid_argument("Replacement column '" + column.name() +
                                    "' has the wrong number of rows");
    columns_[i] = std::move(column);
}

DataFrame DataFrame::take_rows(const std::vector<std::size_t>& rows) const {
    DataFrame out;
    for (const auto& c : columns_) out.add_column(c.take(rows));
    return out;
}

std::vector<CellValue> DataFrame::get_column(const std::string& name) const {
    const Column& c = (*this)[name];
    std::vector<CellValue> out;
    out.reserve(c.size());
    for (std::size_t i = 0; i < c.size(); ++i) {
        if (c.is_numeric())
            out.emplace_back(c.values()[i]);
        else
            out.emplace_back(c.text()[i]);
    }
    return out;
}

void DataFrame::add_column(const std::string& name, const std::vector<CellValue>& data) {
    bool all_numbers = true;
    for (const CellValue& v : data)
        if (!std::holds_alternative<double>(v)) {
            all_numbers = false;
            break;
        }
    if (all_numbers) {
        std::vector<double> numbers;
        numbers.reserve(data.size());
        for (const CellValue& v : data) numbers.push_back(std::get<double>(v));
        add_column(Column(name, std::move(numbers)));
        return;
    }
    std::vector<std::string> strings;
    std::vector<char> missing;
    strings.reserve(data.size());
    missing.reserve(data.size());
    for (const CellValue& v : data) {
        if (std::holds_alternative<double>(v)) {
            strings.push_back(format_number(std::get<double>(v)));
            missing.push_back(0);
        } else {
            const std::string& s = std::get<std::string>(v);
            strings.push_back(s);
            missing.push_back(s.empty() ? 1 : 0);
        }
    }
    add_column(Column(name, std::move(strings), std::move(missing)));
}

std::string DataFrame::to_string(std::size_t max_rows) const {
    const std::size_t shown = std::min(max_rows, n_rows());
    std::vector<std::size_t> width(columns_.size());
    for (std::size_t c = 0; c < columns_.size(); ++c) {
        width[c] = columns_[c].name().size();
        for (std::size_t r = 0; r < shown; ++r)
            width[c] = std::max(width[c], columns_[c].as_string(r).size());
    }
    auto pad = [](const std::string& s, std::size_t w) {
        return s + std::string(w > s.size() ? w - s.size() : 0, ' ');
    };
    std::ostringstream os;
    for (std::size_t c = 0; c < columns_.size(); ++c)
        os << pad(columns_[c].name(), width[c]) << (c + 1 < columns_.size() ? "  " : "");
    os << '\n';
    for (std::size_t r = 0; r < shown; ++r) {
        for (std::size_t c = 0; c < columns_.size(); ++c)
            os << pad(columns_[c].as_string(r), width[c]) << (c + 1 < columns_.size() ? "  " : "");
        os << '\n';
    }
    if (shown < n_rows()) os << "... (" << n_rows() << " rows total)\n";
    return os.str();
}

}  // namespace vizlib
