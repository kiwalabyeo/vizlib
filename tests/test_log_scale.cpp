
#include "vizlib/axes.hpp"
#include "vizlib/ticks.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

int main() {
    using namespace vizlib;

    Axes axes;

    axes.setPlotArea(Rect{10.0, 20.0, 100.0, 200.0});
    axes.setDataRange(1.0, 1000.0, 0.0, 10.0);
    axes.setXScale(ScaleType::Logarithmic);

    Point pixel = axes.dataToPixel(10.0, 5.0);

    assert(std::abs(pixel.x - 43.3333333333) < 1e-6);
    assert(std::abs(pixel.y - 120.0) < 1e-6);

    Point data = axes.pixelToData(pixel.x, pixel.y);

    assert(std::abs(data.x - 10.0) < 1e-6);
    assert(std::abs(data.y - 5.0) < 1e-6);

    Axes yAxes;

    yAxes.setPlotArea(Rect{10.0, 20.0, 100.0, 200.0});
    yAxes.setDataRange(0.0, 10.0, 1.0, 1000.0);
    yAxes.setYScale(ScaleType::Logarithmic);

    Point yPixel = yAxes.dataToPixel(5.0, 10.0);

    assert(std::abs(yPixel.x - 60.0) < 1e-6);
    assert(std::abs(yPixel.y - 153.3333333333) < 1e-6);

    std::vector<double> ticks = generateLogTicks(1.0, 1000.0);

    assert(ticks.size() == 4);
    assert(ticks[0] == 1.0);
    assert(ticks[1] == 10.0);
    assert(ticks[2] == 100.0);
    assert(ticks[3] == 1000.0);

    bool zeroRejected = false;

    try {
        axes.dataToPixel(0.0, 5.0);
    } catch (const std::invalid_argument&) {
        zeroRejected = true;
    }

    assert(zeroRejected);

    bool negativeRejected = false;

    try {
        axes.dataToPixel(-5.0, 5.0);
    } catch (const std::invalid_argument&) {
        negativeRejected = true;
    }

    assert(negativeRejected);

    bool invalidTicksRejected = false;

    try {
        generateLogTicks(-1.0, 100.0);
    } catch (const std::invalid_argument&) {
        invalidTicksRejected = true;
    }

    assert(invalidTicksRejected);

    std::cout << "All logarithmic scaling tests passed!"
              << std::endl;

    return 0;
}
