// Draws a scatter plot of a few hand-picked points and saves it as a PNG.
// Run from the project root: ./build/examples/scatter_example

#include <vizlib/canvas.hpp>
#include <vizlib/image_export.hpp>
#include <vizlib/scatter.hpp>

#include <cstdint>
#include <iostream>
#include <vector>

int main()
{
    const vizlib::Color white{255, 255, 255, 255};
    const vizlib::Color light_grey{235, 235, 235, 255};

    vizlib::Canvas canvas(400, 300, white);

    // grey plot area, 40 px margin all round, so we can see where the dots should stay
    const vizlib::Rect area{40, 40, 320, 220};
    for (int y = 40; y < 260; ++y)
        for (int x = 40; x < 360; ++x)
            canvas.set_pixel(x, y, light_grey);

    // hand-picked points: roughly a rising line with some noise
    const std::vector<double> x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const std::vector<double> y = {2.1, 3.9, 3.2, 5.5, 6.1, 5.8, 7.9, 8.4, 8.0, 9.7};

    vizlib::scatter(canvas, area, x, y);

    // copy the canvas into an RGBA byte array for save_png
    // (to be replaced by Member 7's canvas export adapter once it is merged)
    std::vector<std::uint8_t> bytes;
    bytes.reserve(static_cast<std::size_t>(canvas.width()) * canvas.height() * 4);
    for (int row = 0; row < canvas.height(); ++row)
    {
        for (int col = 0; col < canvas.width(); ++col)
        {
            const vizlib::Color c = canvas.get_pixel(col, row);
            bytes.insert(bytes.end(), {c.r, c.g, c.b, c.a});
        }
    }

    const char* path = "data/output/scatter_example.png";
    if (!vizlib::save_png(path, canvas.width(), canvas.height(), 4, bytes.data()))
    {
        std::cerr << "Could not save " << path << "\n";
        return 1;
    }
    std::cout << "Saved " << path << "\n";
    return 0;
}
