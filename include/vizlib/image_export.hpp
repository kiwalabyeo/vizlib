#pragma once

#include <cstdint>
#include <filesystem>

namespace vizlib
{
    // Saves pixels as a PNG file. Returns true on success.
    // pixels: row-major, top-left first, `channels` bytes per pixel.
    bool save_png(
        const std::filesystem::path& filename,
        int width,
        int height,
        int channels,
        const std::uint8_t* pixels
    );

    // Saves pixels as a PPM (P6) file. Needs channels == 3 (RGB).
    // Returns true on success.
    bool save_ppm(
        const std::filesystem::path& filename,
        int width,
        int height,
        int channels,
        const std::uint8_t* pixels
    );
}