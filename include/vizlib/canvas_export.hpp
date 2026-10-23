#pragma once

#include <filesystem>
#include <vizlib/canvas.hpp>

namespace vizlib
{
    // Saves a whole Canvas as a PNG (keeps transparency, RGBA).
    bool save_png(const Canvas& canvas, const std::filesystem::path& filename);

    // Saves a whole Canvas as a PPM (RGB only, transparency is dropped).
    bool save_ppm(const Canvas& canvas, const std::filesystem::path& filename);
}