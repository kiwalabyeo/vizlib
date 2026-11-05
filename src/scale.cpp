// scale.cpp - LinearScale and LogScale (Member 5). See scale.hpp.

#include "vizlib/scale.hpp"
#include <cmath>
#include <stdexcept>

namespace vizlib {

LinearScale::LinearScale(double dataMin, double dataMax,
                         double pixelMin, double pixelMax) {
    setRange(dataMin, dataMax, pixelMin, pixelMax);
}

void LinearScale::setRange(double dataMin, double dataMax,
                           double pixelMin, double pixelMax) {
    if (!std::isfinite(dataMin) || !std::isfinite(dataMax) ||
        !std::isfinite(pixelMin) || !std::isfinite(pixelMax) ||
        dataMin == dataMax || pixelMin == pixelMax) {
        throw std::invalid_argument("Invalid scale range");
    }

    dataMin_ = dataMin;
    dataMax_ = dataMax;
    pixelMin_ = pixelMin;
    pixelMax_ = pixelMax;
}

// Linear interpolation: how far along the data range the value is (0 to 1),
// times the length of the pixel range, from pixelMin.
// Example: data 0..100 on pixels 50..450, value 50 -> 50 + 0.5 * 400 = 250.
double LinearScale::dataToPixel(double value) const {
    return pixelMin_ +
           (value - dataMin_) / (dataMax_ - dataMin_) *
           (pixelMax_ - pixelMin_);
}

// The same formula with the two ranges swapped: 250 -> 50 again.
double LinearScale::pixelToData(double pixel) const {
    return dataMin_ +
           (pixel - pixelMin_) / (pixelMax_ - pixelMin_) *
           (dataMax_ - dataMin_);
}

LogScale::LogScale(double dataMin, double dataMax,
                   double pixelMin, double pixelMax) {
    setRange(dataMin, dataMax, pixelMin, pixelMax);
}

void LogScale::setRange(double dataMin, double dataMax,
                        double pixelMin, double pixelMax) {
    if (!std::isfinite(dataMin) || !std::isfinite(dataMax) ||
        !std::isfinite(pixelMin) || !std::isfinite(pixelMax) ||
        dataMin <= 0 || dataMax <= 0 ||
        dataMin >= dataMax || pixelMin == pixelMax) {
        throw std::invalid_argument("Invalid logarithmic scale range");
    }

    dataMin_ = dataMin;
    dataMax_ = dataMax;
    pixelMin_ = pixelMin;
    pixelMax_ = pixelMax;
}

// Like LinearScale::dataToPixel, but on log10 of every value.
// Example: data 1..1000 on pixels 10..110, value 10. log10 gives 0, 1 and 3,
// so 10 is one third of the way: 10 + 1/3 * 100 = 43.33.
double LogScale::dataToPixel(double value) const {
    if (!std::isfinite(value) || value <= 0) {
        throw std::invalid_argument(
            "Logarithmic scale requires positive values");
    }

    return pixelMin_ +
           (std::log10(value) - std::log10(dataMin_)) /
           (std::log10(dataMax_) - std::log10(dataMin_)) *
           (pixelMax_ - pixelMin_);
}

double LogScale::pixelToData(double pixel) const {
    if (!std::isfinite(pixel)) {
        throw std::invalid_argument("Invalid pixel value");
    }

    // interpolate in log space, then undo the log with 10^x
    double logValue =
        std::log10(dataMin_) +
        (pixel - pixelMin_) / (pixelMax_ - pixelMin_) *
        (std::log10(dataMax_) - std::log10(dataMin_));

    return std::pow(10.0, logValue);
}

}
