#include "doctest.h"

#include <vizlib/image_export.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace
{
    // Makes a small RGB image: width x height, filled with a simple pattern.
    std::vector<std::uint8_t> make_test_pixels(int width, int height)
    {
        std::vector<std::uint8_t> pixels(static_cast<std::size_t>(width) * height * 3);
        for (std::size_t i = 0; i < pixels.size(); ++i)
        {
            pixels[i] = static_cast<std::uint8_t>(i % 256);
        }
        return pixels;
    }
}

TEST_CASE("save_png writes a valid PNG file")
{
    const int width = 4;
    const int height = 3;
    const auto pixels = make_test_pixels(width, height);
    const std::filesystem::path path = "test_output.png";

    REQUIRE(vizlib::save_png(path, width, height, 3, pixels.data()));

    // The file exists and is not empty.
    REQUIRE(std::filesystem::exists(path));
    CHECK(std::filesystem::file_size(path) > 0);

    // Read the first 24 bytes: PNG signature + width and height.
    std::ifstream file(path, std::ios::binary);
    unsigned char b[24] = {};
    file.read(reinterpret_cast<char*>(b), 24);

    CHECK(b[0] == 0x89);
    CHECK(b[1] == 'P');
    CHECK(b[2] == 'N');
    CHECK(b[3] == 'G');

    const int stored_width  = (b[16] << 24) | (b[17] << 16) | (b[18] << 8) | b[19];
    const int stored_height = (b[20] << 24) | (b[21] << 16) | (b[22] << 8) | b[23];
    CHECK(stored_width == width);
    CHECK(stored_height == height);

    file.close();
    std::filesystem::remove(path);
}

TEST_CASE("save_ppm writes a file of the expected size")
{
    const int width = 4;
    const int height = 3;
    const auto pixels = make_test_pixels(width, height);
    const std::filesystem::path path = "test_output.ppm";

    REQUIRE(vizlib::save_ppm(path, width, height, 3, pixels.data()));
    REQUIRE(std::filesystem::exists(path));

    // Expected size = header text + width * height * 3 bytes.
    const std::string header = "P6\n4 3\n255\n";
    const std::uintmax_t expected = header.size() + static_cast<std::uintmax_t>(width) * height * 3;
    CHECK(std::filesystem::file_size(path) == expected);

    std::filesystem::remove(path);
}

TEST_CASE("saving rejects bad input")
{
    const auto pixels = make_test_pixels(2, 2);

    CHECK_FALSE(vizlib::save_png("bad.png", 0, 2, 3, pixels.data()));      // zero width
    CHECK_FALSE(vizlib::save_png("bad.png", 2, 2, 3, nullptr));            // no pixels
    CHECK_FALSE(vizlib::save_ppm("bad.ppm", 2, 2, 4, pixels.data()));      // PPM needs 3 channels
}