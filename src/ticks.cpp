
#include "vizlib/ticks.hpp"

#include <cmath>
#include <stdexcept>

namespace vizlib {

std::vector<double> generateTicks(double min, double max,
                                  int targetCount) {
    if (!std::isfinite(min) || !std::isfinite(max) ||
        min >= max || targetCount < 2) {
        throw std::invalid_argument("Invalid tick range");
    }

    double rawStep =
        (max - min) / (targetCount - 1);

    double magnitude =
        std::pow(10.0, std::floor(std::log10(rawStep)));

    double fraction = rawStep / magnitude;
    double niceFraction;

    if (fraction <= 1.0) {
        niceFraction = 1.0;
    } else if (fraction <= 2.0) {
        niceFraction = 2.0;
    } else if (fraction <= 5.0) {
        niceFraction = 5.0;
    } else {
        niceFraction = 10.0;
    }

    double step = niceFraction * magnitude;
    double firstTick = std::ceil(min / step) * step;

    std::vector<double> ticks;

    for (double tick = firstTick;
         tick <= max + step * 1e-9;
         tick += step) {
        if (std::abs(tick) < step * 1e-12) {
            tick = 0.0;
        }

        ticks.push_back(tick);
    }

    return ticks;
}

std::vector<double> generateLogTicks(double min, double max) {
    if (!std::isfinite(min) || !std::isfinite(max) ||
        min <= 0 || min >= max) {
        throw std::invalid_argument("Invalid logarithmic tick range");
    }

    std::vector<double> ticks;

    int firstExponent =
        static_cast<int>(std::ceil(std::log10(min)));

    int lastExponent =
        static_cast<int>(std::floor(std::log10(max)));

    for (int exponent = firstExponent;
         exponent <= lastExponent;
         ++exponent) {
        ticks.push_back(std::pow(10.0, exponent));
    }

    return ticks;
}

}

