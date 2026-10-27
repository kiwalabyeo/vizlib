#pragma once

#include <algorithm>
#include <vector>

namespace vizlib {

class summaryStatistics {
    std::vector<double> Data;
    std::vector<double> Sorted_data;

public:
    summaryStatistics(const std::vector<double>& data)
        : Data(data), Sorted_data(data) {
        std::sort(Sorted_data.begin(), Sorted_data.end());
    }

    void collect_data(const std::vector<double>& data) {
        Data = data;
        Sorted_data = data;
        std::sort(Sorted_data.begin(), Sorted_data.end());
    }

    double mean() const;
    double median() const;
    double range() const;
    std::vector<double> mode() const;
    double standard_deviation() const;
    double quantile(double q) const;
    double interquartile_range() const;
    int count(double value) const;
    std::vector<double> histogram_bin_edges() const;
};

} // namespace vizlib