
#include "doctest.h"

#include "vizlib/canvas.hpp"
#include "vizlib/figure.hpp"
#include "vizlib/text.hpp"

#include <filesystem>

TEST_CASE("Figure stores title and axis labels") {
    vizlib::Figure figure;

    figure.setTitle("Monthly Sales");
    figure.setXLabel("Month");
    figure.setYLabel("Revenue");

    CHECK(figure.getTitle() == "Monthly Sales");
    CHECK(figure.getXLabel() == "Month");
    CHECK(figure.getYLabel() == "Revenue");
}

TEST_CASE("Figure draws title and axis labels") {
    const std::filesystem::path fontPath =
        "third_party/fonts/DejaVuSans.ttf";

    vizlib::Canvas canvas(
        800,
        600,
        vizlib::Color{255, 255, 255}
    );

    vizlib::TextRenderer renderer(fontPath);
    vizlib::Figure figure(800, 600);

    figure.setTitle("Monthly Sales");
    figure.setXLabel("Month");
    figure.setYLabel("Revenue");

    figure.drawLabels(canvas, renderer);

    bool foundDarkPixel = false;

    for (int y = 0; y < canvas.height(); ++y) {
        for (int x = 0; x < canvas.width(); ++x) {
            const auto pixel = canvas.get_pixel(x, y);

            if (pixel.r < 200 ||
                pixel.g < 200 ||
                pixel.b < 200) {
                foundDarkPixel = true;
                break;
            }
        }

        if (foundDarkPixel) {
            break;
        }
    }

    CHECK(foundDarkPixel);
}

TEST_CASE("Figure draws no labels when labels are empty") {
    vizlib::Canvas canvas(
        800,
        600,
        vizlib::Color{255, 255, 255}
    );

    vizlib::TextRenderer renderer(
        "third_party/fonts/DejaVuSans.ttf"
    );

    vizlib::Figure figure(800, 600);

    figure.drawLabels(canvas, renderer);

    for (int y = 0; y < canvas.height(); ++y) {
        for (int x = 0; x < canvas.width(); ++x) {
            const auto pixel = canvas.get_pixel(x, y);

            CHECK(pixel.r == 255);
            CHECK(pixel.g == 255);
            CHECK(pixel.b == 255);
        }
    }
}
