// Draws two joint plots of the iris data set (sepal length against petal
// length): one with histogram margins, one with KDE margins. Saves a PNG.
// Run from the project root: ./build/examples/joint_plot_example

#include "example_utils.hpp"

#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/csv_reader.hpp>
#include <vizlib/joint_plot.hpp>
#include <vizlib/kde_plot.hpp>
#include <vizlib/scatter.hpp>
#include <vizlib/text.hpp>

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>

namespace
{
    // Gaussian KDE on 200 evenly spaced points, with Scott's rule for the
    // bandwidth. Temporary: to be replaced by Member 2's KDE in stats once it
    // is merged (the stats module computes KDE values, the plot only draws them).
    void gaussian_kde(const std::vector<double>& data,
                      std::vector<double>& grid, std::vector<double>& density)
    {
        const double n = static_cast<double>(data.size());
        double mean = 0;
        for (double v : data)
            mean += v / n;
        double var = 0;
        for (double v : data)
            var += (v - mean) * (v - mean) / (n - 1);
        const double h = 1.06 * std::sqrt(var) * std::pow(n, -0.2);

        const auto [lo, hi] = std::minmax_element(data.begin(), data.end());
        const double start = *lo - 3 * h;
        const double step = (*hi - *lo + 6 * h) / 199;
        const double pi = std::acos(-1.0);

        grid.clear();
        density.clear();
        for (int i = 0; i < 200; ++i)
        {
            const double g = start + i * step;
            double sum = 0;
            for (double v : data)
                sum += std::exp(-0.5 * ((g - v) / h) * ((g - v) / h));
            grid.push_back(g);
            density.push_back(sum / (n * h * std::sqrt(2 * pi)));
        }
    }

}

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

        // right: the same scatter with KDE margins. JointPlot does not know or
        // care which kind of Plot each panel is.
        std::vector<double> x_grid, x_density, y_grid, y_density;
        gaussian_kde(x, x_grid, x_density);
        gaussian_kde(y, y_grid, y_density);
        vizlib::KdeStyle top_style;
        vizlib::KdeStyle right_style;
        right_style.orientation = vizlib::Orientation::Horizontal;

        const vizlib::JointPlot with_kde(
            std::make_unique<vizlib::ScatterPlot>(x, y, style),
            std::make_unique<vizlib::KdePlot>(x_grid, x_density, top_style),
            std::make_unique<vizlib::KdePlot>(y_grid, y_density, right_style));
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
