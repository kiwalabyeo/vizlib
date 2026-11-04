// Draws a scatter plot of the iris data set (sepal length against petal
// length) and saves it as a PNG.
// Run from the project root: ./build/examples/scatter_example

#include "example_utils.hpp"

#include <vizlib/axes.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/csv_reader.hpp>
#include <vizlib/scatter.hpp>
#include <vizlib/text.hpp>

#include <exception>
#include <iostream>
#include <vector>

int main()
{
    try
    {
        const vizlib::DataFrame iris = vizlib::read_csv("data/input/iris.csv");
        const std::vector<double> x = examples::numeric_column(iris, "sepal_length");
        const std::vector<double> y = examples::numeric_column(iris, "petal_length");

        const vizlib::Color white{255, 255, 255, 255};
        const vizlib::Color light_grey{235, 235, 235, 255};
        const vizlib::Color dark{60, 60, 60, 255};
        vizlib::Canvas canvas(480, 360, white);

        // grey plot area, with room on the left and below for the tick numbers
        const vizlib::Rect area{60, 40, 390, 260};
        canvas.fill_rect(area, light_grey);

        vizlib::Axes axes;
        axes.setPlotArea(area);
        vizlib::ScatterStyle style;
        style.radius = 3;
        vizlib::scatter(canvas, axes, x, y, style);

        // axis lines, tick marks and tick numbers, then a title and axis labels
        const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
        axes.drawAxes(canvas, dark);
        axes.drawTickLabels(canvas, text);
        text.draw_text(canvas, "Iris: sepal length vs petal length", {60, 20}, 16, dark);
        text.draw_text(canvas, "Sepal length (cm)", {200, 345}, 13, dark);
        text.draw_text(canvas, "Petal length (cm)", {8, 36}, 12, dark);

        const char* path = "data/output/scatter_example.png";
        if (!vizlib::save_png(canvas, path))
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
