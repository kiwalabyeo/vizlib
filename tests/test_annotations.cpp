
#include "doctest.h"
#include "vizlib/annotations.hpp"

#include <stdexcept>

TEST_CASE("Annotations stores added text") {
    vizlib::Annotations annotations;

    annotations.add_text("Hello", vizlib::Point{10.0, 20.0});

    const auto& items = annotations.get_annotations();

    REQUIRE(items.size() == 1);
    CHECK(items[0].text == "Hello");
    CHECK(items[0].position.x == 10.0);
    CHECK(items[0].position.y == 20.0);
    CHECK(items[0].fontSize == 12.0);
}

TEST_CASE("Annotations can store custom font size and color") {
    vizlib::Annotations annotations;
    const vizlib::Color red{255, 0, 0};

    annotations.add_text("Title", vizlib::Point{5.0, 8.0}, 18.0, red);

    const auto& item = annotations.get_annotations().at(0);

    CHECK(item.fontSize == 18.0);
    CHECK(item.color.r == 255);
    CHECK(item.color.g == 0);
    CHECK(item.color.b == 0);
}

TEST_CASE("Annotations rejects non-positive font sizes") {
    vizlib::Annotations annotations;

    CHECK_THROWS_AS(
        annotations.add_text("Bad size", vizlib::Point{0.0, 0.0}, 0.0),
        std::invalid_argument
    );

    CHECK(annotations.get_annotations().empty());
}

TEST_CASE("Annotations clear removes all entries") {
    vizlib::Annotations annotations;
    annotations.add_text("First", vizlib::Point{1.0, 2.0});
    annotations.add_text("Second", vizlib::Point{3.0, 4.0});

    annotations.clear();

    CHECK(annotations.get_annotations().empty());
}
