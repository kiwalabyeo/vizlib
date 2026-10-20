// Draws the same data as a vertical and a horizontal histogram, side by side,
// and saves it as a PNG.
// Run from the project root: ./build/examples/histogram_example

#include <vizlib/axes.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/histogram.hpp>
#include <vizlib/image_export.hpp>

#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

int main()
{
    const vizlib::Color white{255, 255, 255, 255};
    const vizlib::Color light_grey{235, 235, 235, 255};

    vizlib::Canvas canvas(700, 300, white);

    // 500 values from a normal distribution, with a fixed seed so the picture
    // is the same every run
    std::mt19937 rng(42);
    std::normal_distribution<double> normal(5.0, 1.5);
    std::vector<double> data;
    for (int i = 0; i < 500; ++i)
        data.push_back(normal(rng));

    // fixed edges for now: 0 to 10 in steps of 0.5
    // (to be replaced by the Freedman–Diaconis edges from stats once fixed)
    std::vector<double> edges;
    for (int i = 0; i <= 20; ++i)
        edges.push_back(i * 0.5);

    // two grey plot areas, 40 px margin all round
    const vizlib::Rect left_area{40, 40, 290, 220};
    const vizlib::Rect right_area{370, 40, 290, 220};
    for (vizlib::Rect r : {left_area, right_area})
        for (int y = static_cast<int>(r.y); y < r.y + r.height; ++y)
            for (int x = static_cast<int>(r.x); x < r.x + r.width; ++x)
                canvas.set_pixel(x, y, light_grey);

    vizlib::Axes vertical_axes;
    vertical_axes.setPlotArea(left_area);
    vizlib::histogram(canvas, vertical_axes, data, edges);

    vizlib::Axes horizontal_axes;
    horizontal_axes.setPlotArea(right_area);
    vizlib::HistogramStyle style;
    style.orientation = vizlib::Orientation::Horizontal;
    vizlib::histogram(canvas, horizontal_axes, data, edges, style);

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

    const char* path = "data/output/histogram_example.png";
    if (!vizlib::save_png(path, canvas.width(), canvas.height(), 4, bytes.data()))
    {
        std::cerr << "Could not save " << path << "\n";
        return 1;
    }
    std::cout << "Saved " << path << "\n";
    return 0;
}
