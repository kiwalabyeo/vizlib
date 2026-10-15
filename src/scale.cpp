
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

}
