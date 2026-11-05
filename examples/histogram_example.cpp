// Draws the same data as a vertical and a horizontal histogram, side by side,
// and saves it as a PNG.
// Run from the project root: ./build/examples/histogram_example

#include <vizlib/axes.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/histogram.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/text.hpp>

#include <iostream>
#include <random>
#include <vector>

int main()
{
    const vizlib::Color white{255, 255, 255, 255};
    const vizlib::Color light_grey{235, 235, 235, 255};

    const vizlib::Color dark{60, 60, 60, 255};
    vizlib::Canvas canvas(780, 360, white);

    // 500 values from a normal distribution, with a fixed seed so the picture
    // is the same every run
    std::mt19937 rng(42);
    std::normal_distribution<double> normal(5.0, 1.5);
    std::vector<double> data;
    for (int i = 0; i < 500; ++i)
        data.push_back(normal(rng));

    // edges chosen by hand: 0 to 10 in steps of 0.5
    // (auto_bin_edges(data) would choose them from the data instead)
    std::vector<double> edges;
    for (int i = 0; i <= 20; ++i)
        edges.push_back(i * 0.5);

    // two grey plot areas, with room on the left and below for the tick numbers
    const vizlib::Rect left_area{60, 40, 300, 260};
    const vizlib::Rect right_area{460, 40, 300, 260};
    canvas.fill_rect(left_area, light_grey);
    canvas.fill_rect(right_area, light_grey);

    vizlib::Axes vertical_axes;
    vertical_axes.setPlotArea(left_area);
    vizlib::histogram(canvas, vertical_axes, data, edges);

    vizlib::Axes horizontal_axes;
    horizontal_axes.setPlotArea(right_area);
    vizlib::HistogramStyle style;
    style.orientation = vizlib::Orientation::Horizontal;
    vizlib::histogram(canvas, horizontal_axes, data, edges, style);

    // axis lines, tick marks and tick numbers on both, then titles and labels
    const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
    for (const vizlib::Axes* axes : {&vertical_axes, &horizontal_axes})
    {
        axes->drawAxes(canvas, dark);
        axes->drawTickLabels(canvas, text);
    }
    text.draw_text(canvas, "Vertical: count per bin", {60, 28}, 16, dark);
    text.draw_text(canvas, "Horizontal: same data, turned", {460, 28}, 16, dark);
    text.draw_text(canvas, "Value", {190, 345}, 13, dark);
    text.draw_text(canvas, "Count", {585, 345}, 13, dark);

    const char* path = "data/output/histogram_example.png";
    if (!vizlib::save_png(canvas, path))
    {
        std::cerr << "Could not save " << path << "\n";
        return 1;
    }
    std::cout << "Saved " << path << "\n";
    return 0;
}
