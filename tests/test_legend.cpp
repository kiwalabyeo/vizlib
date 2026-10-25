#include "doctest.h"
#include "vizlib/legend.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

#include <filesystem>
#include <stdexcept>

TEST_CASE("Legend draws colored entries and labels") {
    const std::filesystem::path fontPath =
        "third_party/fonts/DejaVuSans.ttf";

    vizlib::Canvas canvas(
        200,
        100,
        vizlib::Color{255, 255, 255}
    );

    vizlib::TextRenderer renderer(fontPath);
    vizlib::Legend legend;

    legend.set_position(vizlib::Point{10.0, 10.0});

    legend.add_entry(
        "Temperature",
        vizlib::Color{255, 0, 0}
    );

    legend.add_entry(
        "Pressure",
        vizlib::Color{0, 0, 255}
    );

    legend.draw(canvas, renderer, 12.0);

    // Check that the first colored square is red.
    const auto redPixel = canvas.get_pixel(10, 10);

    CHECK(redPixel.r == 255);
    CHECK(redPixel.g == 0);
    CHECK(redPixel.b == 0);

    // Check that the second colored square is blue.
    const auto bluePixel = canvas.get_pixel(10, 28);

    CHECK(bluePixel.r == 0);
    CHECK(bluePixel.g == 0);
    CHECK(bluePixel.b == 255);

    // Check that the label area contains drawn text.
    bool foundDarkPixel = false;

    for (int y = 10; y < 50; ++y) {
        for (int x = 28; x < canvas.width(); ++x) {
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

TEST_CASE("Legend rejects non-positive font sizes") {
    vizlib::Canvas canvas(
        100,
        50,
        vizlib::Color{255, 255, 255}
    );

    vizlib::TextRenderer renderer(
        "third_party/fonts/DejaVuSans.ttf"
    );

    vizlib::Legend legend;

    CHECK_THROWS_AS(
        legend.draw(canvas, renderer, 0.0),
        std::invalid_argument
    );
}
