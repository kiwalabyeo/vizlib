
#include "doctest.h"

#include "vizlib/annotations.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

#include <filesystem>
#include <stdexcept>

TEST_CASE("Annotations stores text")
{
    vizlib::Annotations annotations;

    annotations.add_text("Hello", vizlib::Point{10, 20});

    const auto& items = annotations.get_annotations();

    REQUIRE(items.size() == 1);
    CHECK(items[0].text == "Hello");
    CHECK(items[0].position.x == 10);
    CHECK(items[0].position.y == 20);
}

TEST_CASE("Annotations stores custom font size and color")
{
    vizlib::Annotations annotations;
    vizlib::Color color{255, 0, 0};

    annotations.add_text(
        "Important",
        vizlib::Point{15, 25},
        18.0,
        color
    );

    const auto& items = annotations.get_annotations();

    REQUIRE(items.size() == 1);
    CHECK(items[0].text == "Important");
    CHECK(items[0].fontSize == 18.0);
    CHECK(items[0].color.r == 255);
    CHECK(items[0].color.g == 0);
    CHECK(items[0].color.b == 0);
}

TEST_CASE("Annotations rejects non-positive font sizes")
{
    vizlib::Annotations annotations;

    CHECK_THROWS_AS(
        annotations.add_text("Invalid", vizlib::Point{0, 0}, 0.0),
        std::invalid_argument
    );

    CHECK_THROWS_AS(
        annotations.add_text("Invalid", vizlib::Point{0, 0}, -1.0),
        std::invalid_argument
    );
}

TEST_CASE("Annotations clears stored text")
{
    vizlib::Annotations annotations;

    annotations.add_text("First", vizlib::Point{10, 10});
    annotations.add_text("Second", vizlib::Point{20, 20});

    annotations.clear();

    CHECK(annotations.get_annotations().empty());
}

TEST_CASE("Annotations draws stored text onto a canvas")
{
    const std::filesystem::path fontPath =
        "third_party/fonts/DejaVuSans.ttf";

    REQUIRE(std::filesystem::exists(fontPath));

    vizlib::TextRenderer renderer(fontPath);

    vizlib::Canvas canvas(
        200,
        60,
        vizlib::Color{255, 255, 255}
    );

    vizlib::Annotations annotations;

    annotations.add_text(
        "Peak value",
        vizlib::Point{10, 35},
        16.0,
        vizlib::Color{0, 0, 0}
    );

    annotations.draw(canvas, renderer);

    bool foundDarkPixel = false;

    for (int y = 0; y < canvas.height(); ++y) {
        for (int x = 0; x < canvas.width(); ++x) {
            const auto pixel = canvas.get_pixel(x, y);

            if (pixel.r < 100 &&
                pixel.g < 100 &&
                pixel.b < 100) {
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
