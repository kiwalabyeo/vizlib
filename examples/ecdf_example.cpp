// ECDF example (Member 3).
// Part 1 prints the ECDF coordinates of a small data set.
// Part 2 draws the ECDF of iris petal length with a rug plot underneath,
// axis lines, tick numbers and labels, and saves data/output/ecdf_example.png.
// Run from the project root: ./build/examples/ecdf_example

#include <vizlib/preprocessing.hpp>

#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/csv_reader.hpp>
#include <vizlib/ecdf.hpp>
#include <vizlib/figure.hpp>
#include <vizlib/text.hpp>

#include <exception>
#include <iostream>
#include <vector>

int main() {
    try {
        // ---- Part 1: the coordinates ----
        std::cout << "ECDF Plot Module Demo\n";

        //  sample data
        std::vector<double> raw_data = {12.5, 5.0, 20.0, 12.5, 8.0, 15.0};

        std::cout << "Input raw data size: " << raw_data.size() << " data points.\n";

        // Creating the ECDFPlot class
        vizlib::ECDFPlot ecdf_plotter;

        // Compute the ECDF coordinates
        std::vector<vizlib::Point> coordinates = ecdf_plotter.compute_coordinates(raw_data);

        // Display the resulting (x, y) step coordinate pairs
        std::cout << "\nComputed ECDF Coordinates (x, cumulative probability y):\n";
        for (std::size_t i = 0; i < coordinates.size(); ++i) {
            std::cout << "Point " << (i + 1)
                      << " -> x: " << coordinates[i].x
                      << ", y: " << coordinates[i].y << "\n";
        }

        // ---- Part 2: the picture ----
        // read the file, then drop rows with a missing value (Member 1's data module)
        const vizlib::DataFrame iris = vizlib::drop_missing(vizlib::read_csv("data/input/iris.csv"));
        const std::vector<double> petal = iris["petal_length"];

        const vizlib::Color white{255, 255, 255, 255};
        const vizlib::Color orange{255, 127, 14, 255};
        vizlib::Canvas canvas(560, 380, white);

        // one plot area; the figure margins leave room for the tick numbers and labels
        vizlib::Figure figure(560, 380);
        vizlib::Axes& axes = figure.addAxes();

        const vizlib::ECDFPlot ecdf(petal);
        vizlib::RugStyle rug_style;
        rug_style.color = orange;
        const vizlib::RugPlot rug(petal, rug_style);

        // the ECDF decides the data range; the rug uses the same axes
        const vizlib::DataBounds b = ecdf.bounds();
        axes.setDataRange(b.x_min, b.x_max, b.y_min, b.y_max);

        // both are Plots: the same two virtual calls draw either kind
        rug.draw(canvas, axes);
        ecdf.draw(canvas, axes);

        const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
        figure.setTitle("ECDF of iris petal length (150 flowers)");
        figure.setXLabel("Petal length (cm)");
        figure.setYLabel("Share of flowers");
        figure.drawAxes(canvas);
        figure.drawLabels(canvas, text);

        const char* path = "data/output/ecdf_example.png";
        if (!vizlib::save_png(canvas, path)) {
            std::cerr << "Could not save " << path << "\n";
            return 1;
        }
        std::cout << "\nSaved " << path << "\n";
        std::cout << "Demo completed successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
