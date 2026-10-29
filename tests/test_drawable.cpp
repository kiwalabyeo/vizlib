
#include "doctest.h"

#include "vizlib/annotations.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/drawable.hpp"
#include "vizlib/legend.hpp"
#include "vizlib/text.hpp"

#include <filesystem>
#include <memory>
#include <vector>

TEST_CASE("Drawable supports runtime polymorphism") {
    const std::filesystem::path fontPath =
        "third_party/fonts/DejaVuSans.ttf";

    vizlib::Canvas canvas(
        200,
        100,
        vizlib::Color{255, 255, 255}
    );

    vizlib::TextRenderer renderer(fontPath);

    vizlib::Annotations annotations;
    annotations.add_text(
        "Peak",
        vizlib::Point{10.0, 30.0}
    );

    vizlib::Legend legend;
    legend.set_position(vizlib::Point{10.0, 50.0});
    legend.add_entry(
        "Temperature",
        vizlib::Color{255, 0, 0}
    );

    std::vector<vizlib::Drawable*> objects{
        &annotations,
        &legend
    };

    REQUIRE(objects.size() == 2);

    for (const vizlib::Drawable* object : objects) {
        REQUIRE(object != nullptr);
        object->draw(canvas, renderer);
    }

    // The calls above should draw both objects without errors.
    const auto redPixel = canvas.get_pixel(10, 50);

    CHECK(redPixel.r == 255);
    CHECK(redPixel.g == 0);
    CHECK(redPixel.b == 0);
}
