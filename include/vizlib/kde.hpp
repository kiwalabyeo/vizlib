#pragma once
#include <vector>

#include "vizlib/concepts.hpp"

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



// KDE of data of any number type (int, float, double, ...). The maths is done in
// double, so the data is converted once and passed to kde_from_doubles.
// bandwidth = 0 means "choose it with Silverman's rule".
template <Numeric T>
KdeResult kde(const std::vector<T>& data, int grid_points = 200, double bandwidth = 0.0) {
    return kde_from_doubles(to_doubles(data), grid_points, bandwidth);
}

}  // namespace vizlib
