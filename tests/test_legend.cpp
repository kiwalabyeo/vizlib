
#include "doctest.h"
#include "vizlib/legend.hpp"

TEST_CASE("Legend stores entries") {
    vizlib::Legend legend;

    legend.add_entry("Temperature", vizlib::Color{255, 0, 0});
    legend.add_entry("Pressure", vizlib::Color{0, 0, 255});

    const auto& entries = legend.get_entries();

    REQUIRE(entries.size() == 2);
    CHECK(entries[0].label == "Temperature");
    CHECK(entries[0].color.r == 255);
    CHECK(entries[1].label == "Pressure");
    CHECK(entries[1].color.b == 255);
}

TEST_CASE("Legend clears entries") {
    vizlib::Legend legend;
    legend.add_entry("Temperature", vizlib::Color{255, 0, 0});

    legend.clear();

    CHECK(legend.get_entries().empty());
}

TEST_CASE("Legend stores and returns its position") {
    vizlib::Legend legend;

    legend.set_position(vizlib::Point{30.0, 40.0});

    const auto position = legend.get_position();

    CHECK(position.x == 30.0);
    CHECK(position.y == 40.0);
}
