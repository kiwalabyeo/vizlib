
#pragma once

namespace vizlib {

class LinearScale {
private:
    double dataMin_;
    double dataMax_;
    double pixelMin_;
    double pixelMax_;

public:
    LinearScale(double dataMin, double dataMax,
                double pixelMin, double pixelMax);

    void setRange(double dataMin, double dataMax,
                  double pixelMin, double pixelMax);

    double dataToPixel(double value) const;
    double pixelToData(double pixel) const;
};


class LogScale {
private:
    double dataMin_;
    double dataMax_;
    double pixelMin_;
    double pixelMax_;

public:
    LogScale(double dataMin, double dataMax,
             double pixelMin, double pixelMax);

    void setRange(double dataMin, double dataMax,
                  double pixelMin, double pixelMax);

    double dataToPixel(double value) const;
    double pixelToData(double pixel) const;
};

}
