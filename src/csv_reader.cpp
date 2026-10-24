#include "vizlib/csv_reader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace vizlib {

DataFrame read_csv(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("CSV file is empty: " + filepath);
    }

    // Parse header
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> headers;
    while (std::getline(ss, item, ',')) {
        headers.push_back(item);
    }

    std::vector<std::vector<CellValue>> raw_data(headers.size());

    // Parse rows
    while (std::getline(file, line)) {
        std::stringstream row_ss(line);
        std::string val_str;
        size_t col_idx = 0;
        while (std::getline(row_ss, val_str, ',')) {
            if (col_idx < headers.size()) {
                // Try parsing as double, otherwise store as string
                try {
                    size_t idx = 0;
                    double d_val = std::stod(val_str, &idx);
                    if (idx == val_str.length()) {
                        raw_data[col_idx].push_back(d_val);
                    } else {
                        raw_data[col_idx].push_back(val_str);
                    }
                } catch (...) {
                    raw_data[col_idx].push_back(val_str);
                }
            }
            col_idx++;
        }
    }

    DataFrame df;
    for (size_t i = 0; i < headers.size(); ++i) {
        df.add_column(headers[i], raw_data[i]);
    }

    return df;
}

} // namespace vizlib