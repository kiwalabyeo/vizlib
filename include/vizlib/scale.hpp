// scale.hpp - LinearScale and LogScale (Member 5)
//
// A scale turns a data value into a pixel position along one axis, and back.
// It knows two ranges: the data range (for example 0 to 100) and the pixel
// range it is drawn over (for example 50 to 450).
//
// The members are private and every range goes through setRange(), which
// checks it. So a scale that exists can never divide by zero, and a LogScale
// can never take the log of 0 or a negative number.

#pragma once

namespace vizlib {

// Equal steps in data give equal steps in pixels.
//     pixel = pixelMin + (value - dataMin) / (dataMax - dataMin) * (pixelMax - pixelMin)
// pixelMax may be smaller than pixelMin: that is how the y axis is flipped
// (bigger data values must appear higher, but pixel y grows downwards).
class LinearScale {
private:
    double dataMin_;
    double dataMax_;
    double pixelMin_;
    double pixelMax_;

public:
    // The constructor calls setRange, so the checks are written only once.
    LinearScale(double dataMin, double dataMax,
                double pixelMin, double pixelMax);

    // Throws std::invalid_argument for NaN or infinity, or an empty range
    // (dataMin == dataMax or pixelMin == pixelMax), which would divide by zero.
    void setRange(double dataMin, double dataMax,
                  double pixelMin, double pixelMax);

    // const: converting only reads the scale, it never changes it
    double dataToPixel(double value) const;
    double pixelToData(double pixel) const;   // the inverse of dataToPixel
};


// Equal RATIOS in data give equal steps in pixels: 1, 10, 100 and 1000 are
// evenly spaced. It is the linear formula applied to log10 of each value.
// Only positive values have a logarithm, so 0 and negative values are refused.
class LogScale {
private:
    double dataMin_;
    double dataMax_;
    double pixelMin_;
    double pixelMax_;

public:
    LogScale(double dataMin, double dataMax,
             double pixelMin, double pixelMax);

    // Throws std::invalid_argument unless 0 < dataMin < dataMax, both finite,
    // and the pixel range is not empty.
    void setRange(double dataMin, double dataMax,
                  double pixelMin, double pixelMax);

    // Throws std::invalid_argument for a value at or below 0 (log10 of 0 is
    // minus infinity, so it has no pixel position).
    double dataToPixel(double value) const;
    // 10 to the power of the interpolated log: gives back the data value
    double pixelToData(double pixel) const;
};

}
