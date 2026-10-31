#pragma once
#include <type_traits>
#include <vector>

namespace vizlib {


struct KdeResult {
    std::vector<double> grid;     // the x positions, evenly spaced
    std::vector<double> density;  // the height of the curve at each x position
    double bandwidth;             // the bandwidth h that was used 
};

// The KDE formula itself, evaluated at ONE point x:
double gaussian_density_at(const std::vector<double>& values, double x, double h);

// Silverman's rule of thumb for choosing the bandwidth h from the data.
double silverman_bandwidth(const std::vector<double>& values);

KdeResult kde_from_doubles(const std::vector<double>& values, int grid_points, double bandwidth);



template <typename T>
KdeResult kde(const std::vector<T>& data, int grid_points = 200, double bandwidth = 0.0) {
concept Numeric=std::integral<T> || std::floating_point<T>;
.
    std::vector<double> values;
    for (T x : data) {
        values.push_back(static_cast<double>(x));
    }
    return kde_from_doubles(values, grid_points, bandwidth);
}

}  // namespace vizlib
