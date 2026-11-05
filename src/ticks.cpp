// ticks.cpp - tick values for the axes (Member 5). See ticks.hpp.

#include "vizlib/ticks.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace vizlib {

std::vector<double> generateTicks(double min, double max,
                                  int targetCount) {
    if (!std::isfinite(min) || !std::isfinite(max) ||
        min >= max || targetCount < 2) {
        throw std::invalid_argument("Invalid tick range");
    }

    // max - min can overflow to infinity for huge ranges (-1e308 to 1e308)
    if (!std::isfinite(max - min)) {
        throw std::invalid_argument("Invalid tick range");
    }

    // Example for generateTicks(0, 20, 5):
    // 1. the plain step that would give exactly targetCount ticks: 20 / 4 = 5
    double rawStep =
        (max - min) / (targetCount - 1);

    // 2. split it into a power of ten and a fraction between 1 and 10:
    //    magnitude = 10^floor(log10 5) = 1, fraction = 5 / 1 = 5
    double magnitude =
        std::pow(10.0, std::floor(std::log10(rawStep)));

    double fraction = rawStep / magnitude;

    // 3. round the fraction UP to 1, 2, 5 or 10, so there are never more
    //    ticks than asked for: 5 stays 5, so the step is 5
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

    // Each tick is k * step for a whole number k. Working out k * step every
    // time, instead of adding step again and again, stops rounding errors
    // from piling up (0.1 + 0.1 + 0.1 is not exactly 0.3).
    const double firstK = std::ceil(min / step - 1e-9);
    const double lastK = std::floor(max / step + 1e-9);

    std::vector<double> ticks;

    for (double k = firstK; k <= lastK; k += 1.0) {
        double tick = k * step;
        // a tick that should be 0 can come out as -1e-17 from rounding;
        // snap it to exactly 0 so its label does not read "-1e-17"
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

    // the smallest and largest powers of 10 inside the range. Example for
    // 2 to 500: ceil(log10 2) = ceil(0.30) = 1 and floor(log10 500) =
    // floor(2.70) = 2, so the ticks are 10^1 = 10 and 10^2 = 100
    int firstExponent =
        static_cast<int>(std::ceil(std::log10(min)));

    int lastExponent =
        static_cast<int>(std::floor(std::log10(max)));

    // A range over many powers of 10 (1e-300 to 1e300) would give hundreds of
    // labels on top of each other: keep about 10 by skipping exponents.
    const int powers = lastExponent - firstExponent + 1;
    const int every = powers > 10 ? (powers + 9) / 10 : 1;

    for (int exponent = firstExponent;
         exponent <= lastExponent;
         exponent += every) {
        ticks.push_back(std::pow(10.0, exponent));
    }

    // A short log range such as 2 to 5 holds no power of 10, and 20 to 50
    // holds only one. An axis needs at least two ticks, so fall back to
    // nice linear ticks inside the range (all above 0, because min > 0).
    if (ticks.size() < 2) {
        return generateTicks(min, max);
    }

    return ticks;
}

// The text for one tick number.
//
// The old way, setprecision(4), cut every number to 4 significant digits, so
// close ticks such as 12345, 12350 and 12355 all printed as "1.234e+04". Here
// the number of decimals comes from the gap between ticks instead: a gap of 5
// needs no decimals, a gap of 0.2 needs one, a gap of 0.05 needs two. Very big
// or very small numbers use scientific notation, with enough digits to tell
// neighbouring ticks apart. Log ticks are powers of 10 and keep the old format.
std::string formatTickLabel(double tick, const std::vector<double>& ticks, bool logScale) {
    std::ostringstream label;
    if (logScale || ticks.size() < 2) {
        label << std::setprecision(4) << tick;
        return label.str();
    }
    const double step = std::abs(ticks[1] - ticks[0]);
    const double biggest = std::max(std::abs(ticks.front()), std::abs(ticks.back()));
    const int stepDigit = static_cast<int>(std::floor(std::log10(step) + 1e-9));
    if (biggest >= 1e6 || biggest < 1e-4) {
        // significant digits from the biggest value down to the step
        const int bigDigit = static_cast<int>(std::floor(std::log10(biggest)));
        const int digits = std::clamp(bigDigit - stepDigit + 1, 1, 17);
        label << std::setprecision(digits) << tick;
    } else {
        label << std::fixed << std::setprecision(std::max(0, -stepDigit)) << tick;
    }
    return label.str();
}


}

