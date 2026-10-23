#include <vizlib/canvas_export.hpp>
#include <vizlib/image_export.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>

namespace vizlib
{
    namespace
    {
        // Copies every pixel of the canvas into a plain byte list.
        // channels = 4 gives R,G,B,A for each pixel; channels = 3 gives R,G,B.
        std::vector<std::uint8_t> to_bytes(const Canvas& canvas, int channels)
        {
            const int w = canvas.width();
            const int h = canvas.height();

            std::vector<std::uint8_t> bytes(
                static_cast<std::size_t>(w) * h * channels);

            std::size_t i = 0;
            for (int y = 0; y < h; ++y)
            {
                for (int x = 0; x < w; ++x)
                {
                    const Color c = canvas.get_pixel(x, y);
                    bytes[i++] = c.r;
                    bytes[i++] = c.g;
                    bytes[i++] = c.b;
                    if (channels == 4)
                    {
                        bytes[i++] = c.a;
                    }
                }
            }
            return bytes;
        }
    }

    bool save_png(const Canvas& canvas, const std::filesystem::path& filename)
    {
        const auto bytes = to_bytes(canvas, 4);
        return save_png(filename, canvas.width(), canvas.height(), 4, bytes.data());
    }

    bool save_ppm(const Canvas& canvas, const std::filesystem::path& filename)
    {
        const auto bytes = to_bytes(canvas, 3);
        return save_ppm(filename, canvas.width(), canvas.height(), 3, bytes.data());
    }
}