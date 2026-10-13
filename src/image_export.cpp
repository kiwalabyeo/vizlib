#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <vizlib/image_export.hpp>

#include <fstream>

namespace vizlib
{

    bool save_png(
        const std::filesystem::path& filename,
        int width,
        int height,
        int channels,
        const std::uint8_t* pixels
    )
    {
        if (width <= 0 || height <= 0 ||
            channels < 1 || channels > 4 ||
            pixels == nullptr)
        {
            return false;
        }

        return stbi_write_png(
            filename.string().c_str(),
            width,
            height,
            channels,
            pixels,
            width * channels
        ) != 0;
    }

    bool save_ppm(
        const std::filesystem::path& filename,
        int width,
        int height,
        int channels,
        const std::uint8_t* pixels
    )
    {
        // PPM (P6) only supports RGB, so we need exactly 3 channels.
        if (width <= 0 || height <= 0 ||
            channels != 3 ||
            pixels == nullptr)
        {
            return false;
        }

        std::ofstream out(filename, std::ios::binary);
        if (!out)
        {
            return false;
        }

        // Header: "P6", then width and height, then the max color value.
        out << "P6\n" << width << " " << height << "\n255\n";

        // Then all the pixel bytes, row by row.
        out.write(
            reinterpret_cast<const char*>(pixels),
            static_cast<std::streamsize>(width) * height * channels
        );

        return static_cast<bool>(out);
    }

}