#pragma once

#include <algorithm>
#include <vector>
#include <stdexcept>

namespace vizlib {

class summaryStatistics {
    std::vector<double> Data;
    std::vector<double> Sorted_data;

public:
    summaryStatistics(const std::vector<double>& data)
    {
        collect_data(data);
    }

    void collect_data(const std::vector<double>& data) {
        if (data.empty()) {
            throw std::invalid_argument("Data cannot be empty");
        }
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
class binningStrategy{
protected:
    std::vector<double> Data;
    std::vector<double> Sorted_data;
public:
    binningStrategy(const std::vector<double>& Data):
        Data(Data), Sorted_data(Data) {
        std::sort(Sorted_data.begin(), Sorted_data.end());
    }
    void collect_data(const std::vector<double>& data) {
        Data = data;
        Sorted_data = data;
        std::sort(Sorted_data.begin(), Sorted_data.end());
    }
    virtual std::vector<double> histogram_bin_edges() const = 0;
    virtual ~binningStrategy(){};
};
class FreedmanDiaconisBinning : public binningStrategy{
public:
    FreedmanDiaconisBinning(const std::vector<double>& Data): binningStrategy(Data) {}
    std::vector<double> histogram_bin_edges() const override;
};
class SturgesBinning : public binningStrategy{
public: 
    SturgesBinning(const std::vector<double>& Data): binningStrategy(Data) {}
    std::vector<double> histogram_bin_edges() const override;
};
} // namespace vizlib