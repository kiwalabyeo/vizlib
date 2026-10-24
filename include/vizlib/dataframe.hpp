#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <variant>

namespace vizlib {

using CellValue = std::variant<double, std::string>;

class DataFrame {
public:
    void add_column(const std::string& name, const std::vector<CellValue>& data);
    const std::vector<CellValue>& get_column(const std::string& name) const;
    size_t row_count() const;
    size_t column_count() const;

private:
    std::unordered_map<std::string, std::vector<CellValue>> columns_;
    size_t rows_ = 0;
};

} // namespace vizlib