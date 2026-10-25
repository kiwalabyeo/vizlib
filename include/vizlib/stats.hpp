#pragma once

#include <vector>

namespace vizlib {

class summaryStatistics {
    std::vector<double> data;
public:
    summaryStatistics(const std::vector<double>& data) : data(data) {}
    void collect_data(const std::vector<double>& data) {
        this->data = data;
    }
    double mean(const std::vector<double>& data);
    const std::vector<double> sorted(const std::vector<double>& data);
    double median(const std::vector<double>& sorted_data);
    double range(const std::vector<double>& sorted_data);
    std::vector<double> mode(const std::vector<double>& data);
    double standard_deviation(const std::vector<double>& data);
    double quantile(const std::vector<double>& sorted_data, double q);
    double interquartile_range(const std::vector<double>& sorted_data);
    int count(const std::vector<double>& data, double value);
    std::vector<double> histogram_bin_edges(const std::vector<double>& data);
};

} // namespace vizlib
