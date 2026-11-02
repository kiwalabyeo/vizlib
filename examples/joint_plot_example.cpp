// Draws two joint plots of the iris data set (sepal length against petal
// length): one with histogram margins, one with KDE margins. Saves a PNG.
// Run from the project root: ./build/examples/joint_plot_example

#include "example_utils.hpp"

#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/csv_reader.hpp>
#include <vizlib/joint_plot.hpp>
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
        const vizlib::Color dark{40, 40, 40, 255};
        vizlib::Canvas canvas(900, 460, white);
        vizlib::ScatterStyle style;
        style.radius = 3;

        // left: the usual joint plot, scatter with histogram margins
        // (Freedman–Diaconis bins, worked out by auto_bin_edges)
        const vizlib::JointPlot with_histograms = vizlib::JointPlot::with_histograms(x, y, style);
        with_histograms.draw(canvas, {30, 50, 400, 380});

        // right: the same scatter with KDE margins. The curves come from
        // Member 2's kde() (Gaussian kernel, Silverman's bandwidth).
        const vizlib::JointPlot with_kde = vizlib::JointPlot::with_kde(x, y, style);
        with_kde.draw(canvas, {470, 50, 400, 380});

        const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
        text.draw_text(canvas, "Iris: histogram margins", {30, 30}, 16, dark);
        text.draw_text(canvas, "Iris: KDE margins", {470, 30}, 16, dark);
        text.draw_text(canvas, "x: sepal length    y: petal length    (150 flowers)", {30, 452}, 12, dark);

        const char* path = "data/output/joint_plot_example.png";
        if (!vizlib::save_png(canvas, path))
        {
            std::cerr << "Could not save " << path << "\n";
            return 1;
        }
        std::cout << "Saved " << path << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
