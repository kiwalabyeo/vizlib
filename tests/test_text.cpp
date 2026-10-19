
#include "doctest.h"

#include "vizlib/text.hpp"

#include <filesystem>
#include <stdexcept>

TEST_CASE("TextRenderer measures text")
{
    const std::filesystem::path fontPath =
        "third_party/fonts/DejaVuSans.ttf";

    vizlib::TextRenderer text(fontPath);

    const auto metrics = text.measure_text("Hello", 16.0);

    CHECK(metrics.width > 0.0);
    CHECK(metrics.height > 0.0);
}

TEST_CASE("TextRenderer draws text onto a canvas")
{
    const std::filesystem::path fontPath =
        "third_party/fonts/DejaVuSans.ttf";

    vizlib::Canvas canvas(200, 60, vizlib::Color{255, 255, 255});
    vizlib::TextRenderer text(fontPath);

    text.draw_text(
        canvas,
        "Hello",
        vizlib::Point{10.0, 30.0},
        16.0,
        vizlib::Color{0, 0, 0}
    );

    bool foundDarkPixel = false;

    for (int y = 0; y < canvas.height(); ++y) {
        for (int x = 0; x < canvas.width(); ++x) {
            const auto pixel = canvas.get_pixel(x, y);

            if (pixel.r < 200 || pixel.g < 200 || pixel.b < 200) {
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

TEST_CASE("TextRenderer rejects a missing font")
{
    CHECK_THROWS_AS(
        vizlib::TextRenderer("missing-font.ttf"),
        std::runtime_error
    );
}
