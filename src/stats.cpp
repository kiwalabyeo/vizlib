#include "vizlib/stats.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace vizlib {

double summaryStatistics::mean() const {

    double sum = 0;
    for (double number : Data) {
        sum += number;
    }
    return sum / Data.size();
}

double summaryStatistics::median() const {

    std::size_t n = Sorted_data.size();
    if (n % 2 == 0) {
        return (Sorted_data[n / 2 - 1] + Sorted_data[n / 2]) / 2;
    }
    return Sorted_data[(n - 1) / 2];
}

double summaryStatistics::range() const {

    return Sorted_data.back() - Sorted_data.front();
}

std::vector<double> summaryStatistics::mode() const {
    std::unordered_map<double, int> count;
    for (double number : Data) {  // maps each number to its frequency
        count[number]++;
    }
    int max_freq = 0;
    for (const auto& entry : count) {  // finds the maximum frequency
        if (entry.second > max_freq) {
            max_freq = entry.second;
        }
    }
    std::vector<double> modes;
    for (const auto& entry : count) {  // collects all numbers with that frequency
        if (entry.second == max_freq) {
            modes.push_back(entry.first);
        }
    }
    std::sort(modes.begin(), modes.end());
    return modes;
}

double summaryStatistics::standard_deviation() const {
    double mean_value = mean();
    double sum_of_square_diffs = 0;
    for (double number : Data) {
        sum_of_square_diffs += (number - mean_value) * (number - mean_value);
    }
    return std::sqrt(sum_of_square_diffs / Data.size());
}

double summaryStatistics::quantile(double q) const {  // q must be between 0 and 1
    if (q < 0 || q > 1) {
        throw std::invalid_argument("q must be between 0 and 1");
    }


    double rank = q * (Sorted_data.size() - 1);/*The rank is calculated as q * (n - 1), where n is the number of data points.
     This gives the position in the sorted data corresponding to the desired quantile. 
     This formular is different from the (n+1) formula used in some other implementations, because c++ used zero indexed arrays */
    int lower_index = static_cast<int>(std::floor(rank));
    int upper_index = static_cast<int>(std::ceil(rank));

    if (lower_index == upper_index) {
        return Sorted_data[lower_index];
    }

    // Linear interpolation
    double weight = rank - lower_index;
    return Sorted_data[lower_index] * (1.0 - weight) + Sorted_data[upper_index] * weight;
}

double summaryStatistics::interquartile_range() const {
    return quantile(0.75) - quantile(0.25);
}

int summaryStatistics::count(double value) const {
    int total = 0;
    for (double number : Data) {
        if (number == value) {
            total++;
        }
    }
    return total;
}

std::vector<double> FreedmanDiaconisBinning::histogram_bin_edges() const {
    std::vector<double> bin_edges;

    // Freedman-Diaconis rule for bin width
    double bin_width = 2 * summaryStatistics(Sorted_data).interquartile_range() / std::cbrt(static_cast<double>(Sorted_data.size()));
    if (bin_width == 0) {
        bin_edges.push_back(Sorted_data.front());
        return bin_edges;
    }

    int number_of_bins = static_cast<int>(std::ceil(summaryStatistics(Sorted_data).range() / bin_width));
    for (int i = 0; i <= number_of_bins; i++) {
        bin_edges.push_back(Sorted_data.front() + i * bin_width);
    }
    return bin_edges;
}
std::vector<double> SturgesBinning::histogram_bin_edges() const {
    std::vector<double> bin_edges;
    // Sturges' formula for number of bins
    int number_of_bins = static_cast<int>(std::ceil( 1 + std::log2(Sorted_data.size())));
    double bin_width = summaryStatistics(Sorted_data).range() / number_of_bins;

    for (int i = 0; i <= number_of_bins; i++) {
        bin_edges.push_back(Sorted_data.front() + i * bin_width);
    }
    return bin_edges;

}
std::vector<double> summaryStatistics::histogram_bin_edges()const{
    if (Sorted_data.size()>30&&Sorted_data.size()<200) {       //Sturges rule works best for moderate sized sample datasets
        return SturgesBinning(Sorted_data).histogram_bin_edges();
    }
    else {
        return FreedmanDiaconisBinning(Sorted_data).histogram_bin_edges();
    }
} } // namespace vizlib