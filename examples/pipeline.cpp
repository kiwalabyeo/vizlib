// First version of the vizlib pipeline.
// Generates sample data, draws a scatter plot and a histogram side by side,
// and saves the picture as a PNG.
// Run from the project root: ./build/examples/pipeline

#include <vizlib/axes.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/histogram.hpp>
#include <vizlib/scatter.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

namespace
{
    // Paints a plot area so we can see where the plot should stay.
    void fill_area(vizlib::Canvas& canvas, const vizlib::Rect& r,
                   const vizlib::Color& c)
    {
        for (int y = static_cast<int>(r.y); y < r.y + r.height; ++y)
            for (int x = static_cast<int>(r.x); x < r.x + r.width; ++x)
                canvas.set_pixel(x, y, c);
    }
}

int main()
{
    // Step 1: data. Later this will come from the CSV reader.
    // A fixed seed gives the same picture every run.
    std::mt19937 rng(42);
    std::normal_distribution<double> noise(0.0, 2.0);
    std::vector<double> x;
    std::vector<double> y;
    for (int i = 1; i <= 60; ++i)
    {
        x.push_back(i);
        y.push_back(0.5 * i + noise(rng));   // a rising line with noise
    }

    // Step 2: 10 equal bins from the smallest to the largest y value.
    // Later this will use the bin edges from stats.
    const auto range = std::minmax_element(y.begin(), y.end());
    const double low = *range.first;
    const double high = *range.second;
    std::vector<double> edges;
    for (int i = 0; i <= 10; ++i)
        edges.push_back(low + (high - low) * i / 10.0);

    // Step 3: canvas with two plot areas side by side
    const vizlib::Color white{255, 255, 255, 255};
    const vizlib::Color light_grey{235, 235, 235, 255};
    vizlib::Canvas canvas(700, 300, white);

    const vizlib::Rect left_area{40, 40, 290, 220};
    const vizlib::Rect right_area{370, 40, 290, 220};
    fill_area(canvas, left_area, light_grey);
    fill_area(canvas, right_area, light_grey);

    // Step 4: draw the two plots
    vizlib::scatter(canvas, left_area, x, y);

    vizlib::Axes axes;
    axes.setPlotArea(right_area);
    vizlib::histogram(canvas, axes, y, edges);

    // Step 5: save with the Canvas export adapter (one line)
    const char* path = "data/output/pipeline.png";
    if (!vizlib::save_png(canvas, path))
    {
        std::cerr << "Could not save " << path << "\n";
        return 1;
    }
    std::cout << "Saved " << path << "\n";
    return 0;
}