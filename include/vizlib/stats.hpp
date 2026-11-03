#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include <stdexcept>

namespace vizlib {

// The values that are not NaN. A missing value in a CSV column is stored as
// NaN, so NaN means "no value here" and is left out of every statistic.
// std::sort also needs values that can be compared, and NaN cannot be.
// Infinity is refused, because no mean, spread or bin can be worked out from it.
inline std::vector<double> values_without_nan(const std::vector<double>& data) {
    std::vector<double> values;
    values.reserve(data.size());
    for (double v : data) {
        if (std::isnan(v)) {
            continue;
        }
        if (std::isinf(v)) {
            throw std::invalid_argument("Data must not contain infinity");
        }
        values.push_back(v);
    }
    return values;
}

class summaryStatistics {
    std::vector<double> Data;
    std::vector<double> Sorted_data;

public:
    summaryStatistics(const std::vector<double>& data)
    {
        collect_data(data);
    }

    // NaN (missing) values are left out. Throws if no value is left.
    void collect_data(const std::vector<double>& data) {
        if (data.empty()) {
            throw std::invalid_argument("Data cannot be empty");
        }
        std::vector<double> values = values_without_nan(data);
        if (values.empty()) {
            throw std::invalid_argument("Data has no values that are not NaN");
        }
        Data = values;
        Sorted_data = values;
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
class binningStrategy{
protected:
    std::vector<double> Data;
    std::vector<double> Sorted_data;

    // the most bins a rule may make: one far outlier would otherwise ask
    // Freedman-Diaconis for millions of bins
    static constexpr int max_bins = 1000;

    // when every value is equal there is no range to split: one bin of
    // width 1 around the value
    std::vector<double> single_bin() const {
        return {Sorted_data.front() - 0.5, Sorted_data.front() + 0.5};
    }
public:
    binningStrategy(const std::vector<double>& data) {
        collect_data(data);
    }
    // NaN (missing) values are left out. Throws if no value is left.
    void collect_data(const std::vector<double>& data) {
        std::vector<double> values = values_without_nan(data);
        if (values.empty()) {
            throw std::invalid_argument("Data has no values that are not NaN");
        }
        Data = values;
        Sorted_data = values;
        std::sort(Sorted_data.begin(), Sorted_data.end());
    }
    virtual std::vector<double> histogram_bin_edges() const = 0;
    virtual ~binningStrategy(){};
};
class FreedmanDiaconisBinning : public binningStrategy{
public:
    FreedmanDiaconisBinning(const std::vector<double>& data): binningStrategy(data) {}
    std::vector<double> histogram_bin_edges() const override;
};
class SturgesBinning : public binningStrategy{
public: 
    SturgesBinning(const std::vector<double>& data): binningStrategy(data) {}
    std::vector<double> histogram_bin_edges() const override;
};
} // namespace vizlib