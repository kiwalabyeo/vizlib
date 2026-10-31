
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

double LinearScale::dataToPixel(double value) const {
    return pixelMin_ +
           (value - dataMin_) / (dataMax_ - dataMin_) *
           (pixelMax_ - pixelMin_);
}

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

    double logValue =
        std::log10(dataMin_) +
        (pixel - pixelMin_) / (pixelMax_ - pixelMin_) *
        (std::log10(dataMax_) - std::log10(dataMin_));

    return std::pow(10.0, logValue);
}

}
