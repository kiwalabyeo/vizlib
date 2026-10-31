#include "kde.hpp"
#include "stats.hpp"
#include <cmath>
#include <stdexcept>

namespace vizlib {

double gaussian_density_at(const std::vector<double>& values, double x, double h) {
    if (values.empty()) {
        throw std::invalid_argument("data cannot be empty");
    }
    if (h <= 0) {
        throw std::invalid_argument("bandwidth must be bigger than 0");
    }

    const double PI = 3.14159265358979323846; 

    // Add up one bell-curve "bump" per data point.
    double sum = 0;
    for (double xi : values) {
        double u = (x - xi) / h;      // how many bandwidths away from this data point is x?
        sum += std::exp(-0.5 * u * u);  // the Gaussian shape, without its constant
    }

    // The constants: 1/sqrt(2*pi) belongs to the Gaussian kernel, and 1/(n*h) makes the
    // total area under the curve equal to 1.
    return sum / (values.size() * h * std::sqrt(2 * PI));
}

double silverman_bandwidth(const std::vector<double>& values) {
    summaryStatistics stats(values);  // throws if values is empty

    // For bell-shaped data the standard deviation is a good measure of spread. The IQR
    // divided by 1.34 estimates the same thing but ignores outliers. Use the smaller one.
    // (1.34 comes from the normal curve: its IQR is about 1.34 standard deviations.)
    double spread = stats.standard_deviation();
    double iqr = stats.interquartile_range();
    if (iqr > 0 && iqr / 1.34 < spread) {
        spread = iqr / 1.34;
    }

    // All values identical: no spread to measure, so use 1 so that h is still above 0.
    if (spread == 0) {
        spread = 1.0;
    }

    // Silverman's rule: h = 0.9 * spread * n^(-1/5)
    return 0.9 * spread * std::pow(values.size(), -0.2);
}

KdeResult kde_from_doubles(const std::vector<double>& values, int grid_points, double bandwidth) {
    if (grid_points < 2) {
        throw std::invalid_argument("grid_points must be at least 2");
    }
    if (bandwidth < 0) {
        throw std::invalid_argument("bandwidth cannot be negative");
    }

    summaryStatistics stats(values);  // throws if values is empty; gives us minimum/maximum

    double h = bandwidth;
    if (h == 0) {
        h = silverman_bandwidth(values);  // 0 means "choose it for me"
    }

    // The curve is calculated from 3 bandwidths below the smallest value to 3 above the
    // largest, so the tails fade out instead of being cut off.
    double maximum=values[0];
    double minimum=values[0];
    for(double v:values){
        if(v<minimum){
            minimum = v;
        }
    }
      for(double v:values){
        if(v>maximum){
            maximum = v;
        }
    }
    double start = minimum - 3 * h;
    double end = maximum + 3 * h;
    double step = (end - start) / (grid_points - 1);

    KdeResult result;
    result.bandwidth = h;
    for (int i = 0; i < grid_points; i++) {
        double x = start + i * step;
        result.grid.push_back(x);
        result.density.push_back(gaussian_density_at(values, x, h));
    }
    return result;
}

}  // namespace vizlib
