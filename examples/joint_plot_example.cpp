// Draws two joint plots of the iris data set (sepal length against petal
// length): one with histogram margins, one with KDE margins. Saves a PNG.
// Run from the project root: ./build/examples/joint_plot_example

#include <vizlib/preprocessing.hpp>

#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/csv_reader.hpp>
#include <vizlib/joint_plot.hpp>
#include <vizlib/scatter.hpp>
#include <vizlib/text.hpp>

#include <exception>
#include <utility>
#include <iostream>
#include <vector>

int main()
{
    try
    {
        // read the file, then drop rows with a missing value (Member 1's data module)
        const vizlib::DataFrame iris = vizlib::drop_missing(vizlib::read_csv("data/input/iris.csv"));
        const std::vector<double> x = iris["sepal_length"];
        const std::vector<double> y = iris["petal_length"];

        const vizlib::Color white{255, 255, 255, 255};
        const vizlib::Color dark{40, 40, 40, 255};
        const vizlib::Color axis_grey{60, 60, 60, 255};
        vizlib::Canvas canvas(940, 490, white);
        const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
        vizlib::ScatterStyle style;
        style.radius = 3;

        // left: the usual joint plot, scatter with histogram margins
        // (bins chosen by auto_bin_edges from Member 2's binning rules)
        const vizlib::JointPlot with_histograms = vizlib::JointPlot::with_histograms(x, y, style);
        const vizlib::Rect left{55, 50, 390, 380};
        with_histograms.draw(canvas, left);

        // right: the same scatter with KDE margins. The curves come from
        // Member 2's kde() (Gaussian kernel, Silverman's bandwidth).
        const vizlib::JointPlot with_kde = vizlib::JointPlot::with_kde(x, y, style);
        const vizlib::Rect right{535, 50, 390, 380};
        with_kde.draw(canvas, right);

        // axes on the centre panel of each joint plot: layout() gives the same
        // Axes that draw() used, so the ticks line up with the dots. The margin
        // panels only show the shape of the spread, so they get no ticks.
        for (const auto& [plot, area] : {std::pair{&with_histograms, left}, std::pair{&with_kde, right}})
        {
            const vizlib::JointAxes axes = plot->layout(area);
            axes.center.drawAxes(canvas, axis_grey);
            axes.center.drawTickLabels(canvas, text);
        }

        text.draw_text(canvas, "Iris: histogram margins", {55, 30}, 16, dark);
        text.draw_text(canvas, "Iris: KDE margins", {535, 30}, 16, dark);
        text.draw_text(canvas, "x: sepal length (cm)    y: petal length (cm)    150 flowers", {55, 478}, 12, dark);

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
