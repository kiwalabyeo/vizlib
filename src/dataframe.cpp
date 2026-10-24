#include "vizlib/dataframe.hpp"
#include <stdexcept>

namespace vizlib {

void DataFrame::add_column(const std::string& name, const std::vector<CellValue>& data) {
    if (!columns_.empty() && data.size() != rows_) {
        throw std::invalid_argument("Column size does not match existing row count.");
    }
    columns_[name] = data;
    rows_ = data.size();
}

const std::vector<CellValue>& DataFrame::get_column(const std::string& name) const {
    return columns_.at(name);
}

size_t DataFrame::row_count() const {
    return rows_;
}

size_t DataFrame::column_count() const {
    return columns_.size();
}

} // namespace vizlib