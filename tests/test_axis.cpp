
#include "vizlib/axes.hpp"
#include "vizlib/figure.hpp"
#include "vizlib/scale.hpp"
#include <iostream>

int main() {
    vizlib::LinearScale scale(0, 100, 50, 450);

    std::cout << "Data to pixel: "
              << scale.dataToPixel(50) << '\n';

    std::cout << "Pixel to data: "
              << scale.pixelToData(250) << '\n';

    vizlib::Figure figure(800, 600);

    vizlib::Axes& axes = figure.addAxes();

    axes.setDataRange(0, 100, 0, 50);

    vizlib::Point point = axes.dataToPixel(50, 25);

    std::cout << "X pixel: " << point.x << '\n';
    std::cout << "Y pixel: " << point.y << '\n';

    std::cout << "Number of axes: "
              << figure.getAxesCount() << '\n';

    return 0;
}
