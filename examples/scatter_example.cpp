// Draws a scatter plot of the iris data set (sepal length against petal
// length) and saves it as a PNG.
// Run from the project root: ./build/examples/scatter_example

#include "example_utils.hpp"

#include <vizlib/axes.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/scatter.hpp>

#include <exception>
#include <iostream>
#include <vector>

int main()
{
    try
    {
        const std::vector<double> x = examples::read_csv_column("data/input/iris.csv", "sepal_length");
        const std::vector<double> y = examples::read_csv_column("data/input/iris.csv", "petal_length");

        const vizlib::Color white{255, 255, 255, 255};
        const vizlib::Color light_grey{235, 235, 235, 255};
        vizlib::Canvas canvas(400, 300, white);

        // grey plot area, 40 px margin all round, so we can see where the dots should stay
        const vizlib::Rect area{40, 40, 320, 220};
        for (int y_px = 40; y_px < 260; ++y_px)
            for (int x_px = 40; x_px < 360; ++x_px)
                canvas.set_pixel(x_px, y_px, light_grey);

        vizlib::Axes axes;
        axes.setPlotArea(area);
        vizlib::ScatterStyle style;
        style.radius = 3;
        vizlib::scatter(canvas, axes, x, y, style);

        const char* path = "data/output/scatter_example.png";
        if (!examples::save_canvas_png(canvas, path))
        {
            std::cerr << "Could not save " << path << "\n";
            return 1;
        }
        std::cout << "Plotted " << x.size() << " iris flowers. Saved " << path << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
