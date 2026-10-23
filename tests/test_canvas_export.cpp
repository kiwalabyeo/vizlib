#include "doctest.h"

#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

TEST_CASE("a real Canvas is saved as PNG with the right size")
{
    vizlib::Canvas canvas(4, 3, vizlib::Color{0, 0, 255, 255});   // blue background
    canvas.set_pixel(0, 0, vizlib::Color{255, 0, 0, 255});        // red pixel, top-left

    const std::filesystem::path path = "canvas_test.png";
    REQUIRE(vizlib::save_png(canvas, path));
    REQUIRE(std::filesystem::exists(path));
    CHECK(std::filesystem::file_size(path) > 0);

    // PNG signature and the stored width and height
    std::ifstream file(path, std::ios::binary);
    unsigned char b[24] = {};
    file.read(reinterpret_cast<char*>(b), 24);
    CHECK(b[1] == 'P');
    CHECK(b[2] == 'N');
    CHECK(b[3] == 'G');
    CHECK(((b[16] << 24) | (b[17] << 16) | (b[18] << 8) | b[19]) == 4);
    CHECK(((b[20] << 24) | (b[21] << 16) | (b[22] << 8) | b[23]) == 3);

    file.close();
    std::filesystem::remove(path);
}

TEST_CASE("a real Canvas is saved as PPM, top-left pixel first")
{
    vizlib::Canvas canvas(4, 3, vizlib::Color{0, 0, 255, 255});
    canvas.set_pixel(0, 0, vizlib::Color{255, 0, 0, 255});

    const std::filesystem::path path = "canvas_test.ppm";
    REQUIRE(vizlib::save_ppm(canvas, path));

    const std::string header = "P6\n4 3\n255\n";
    CHECK(std::filesystem::file_size(path) == header.size() + 4 * 3 * 3);

    // Read the bytes after the header
    std::ifstream file(path, std::ios::binary);
    std::vector<unsigned char> data(header.size() + 6);
    file.read(reinterpret_cast<char*>(data.data()),
              static_cast<std::streamsize>(data.size()));

    // First pixel = red (the top-left one we set)
    CHECK(data[header.size() + 0] == 255);
    CHECK(data[header.size() + 1] == 0);
    CHECK(data[header.size() + 2] == 0);
    // Second pixel = blue (the background)
    CHECK(data[header.size() + 3] == 0);
    CHECK(data[header.size() + 4] == 0);
    CHECK(data[header.size() + 5] == 255);

    file.close();
    std::filesystem::remove(path);
}