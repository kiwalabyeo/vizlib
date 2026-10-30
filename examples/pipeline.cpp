
// Vizlib pipeline example.
// Reads Iris data, draws a scatter plot and histogram side by side,
// adds titles, axis labels, tick labels, an annotation and a legend,
// and saves the completed figure as a PNG.
//
// Run from the project root: ./build/examples/pipeline

#include <vizlib/figure.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/histogram.hpp>
#include <vizlib/scatter.hpp>
#include <vizlib/stats.hpp>
#include <vizlib/text.hpp>
#include <vizlib/annotations.hpp>
#include <vizlib/legend.hpp>
#include "vizlib/csv_reader.hpp"

#include <iostream>
#include <vector>
#include <variant>

namespace
{
    // Paints a plot area with a background colour.
    void fill_area(vizlib::Canvas& canvas,
                   const vizlib::Rect& r,
                   const vizlib::Color& c)
    {
        for (int y = static_cast<int>(r.y);
             y < r.y + r.height; ++y)
        {
            for (int x = static_cast<int>(r.x);
                 x < r.x + r.width; ++x)
            {
                canvas.set_pixel(x, y, c);
            }
        }
    }

    // Converts a DataFrame column into a list of numbers.
    // Text cells are skipped.
    std::vector<double> column_as_doubles(
        const vizlib::DataFrame& df,
        const std::string& name)
    {
        std::vector<double> values;

        for (const vizlib::CellValue& cell : df.get_column(name))
        {
            if (const double* number = std::get_if<double>(&cell))
            {
                values.push_back(*number);
            }
        }

        return values;
    }
}

int main()
{
    // Step 1: Read data from the CSV file.
    const vizlib::DataFrame df =
        vizlib::read_csv("data/input/iris.csv");

    const std::vector<double> x =
        column_as_doubles(df, "sepal_length");

    const std::vector<double> y =
        column_as_doubles(df, "petal_length");

    if (x.empty() || y.empty() || x.size() != y.size())
    {
        std::cerr << "Could not load valid plot data.\n";
        return 1;
    }

    // Step 2: Calculate histogram bin edges.
    vizlib::summaryStatistics stats(y);
    const std::vector<double> edges =
        stats.histogram_bin_edges();

    // Step 3: Define colours and create the canvas.
    const vizlib::Color white{255, 255, 255, 255};
    const vizlib::Color light_grey{235, 235, 235, 255};
    const vizlib::Color black{0, 0, 0, 255};
    const vizlib::Color blue{31, 119, 180, 255};

    vizlib::Canvas canvas(700, 360, white);

    // Step 4: Create a figure with two axes areas.
    vizlib::Figure figure(700, 360);

    vizlib::Axes& scatter_axes =
        figure.addAxes(vizlib::Rect{0, 0, 340, 300});

    vizlib::Axes& histogram_axes =
        figure.addAxes(vizlib::Rect{360, 0, 340, 300});

    // Step 5: Get the actual plot areas and colour their backgrounds.
    const vizlib::Rect left_area =
        scatter_axes.getPlotArea();

    const vizlib::Rect right_area =
        histogram_axes.getPlotArea();

    fill_area(canvas, left_area, light_grey);
    fill_area(canvas, right_area, light_grey);

    // Step 6: Draw the scatter plot.
    vizlib::scatter(canvas, scatter_axes, x, y);

    // Step 7: Draw the histogram.
    vizlib::histogram(canvas, histogram_axes, y, edges);

    // Step 8: Load the text renderer.
    const vizlib::TextRenderer text(
        "third_party/fonts/DejaVuSans.ttf");

    // Step 9: Add the scatter plot title and axis labels.
    figure.setTitle("Scatter Plot");
    figure.setXLabel("Sepal Length");
    figure.setYLabel("Petal Length");

    // Step 10: Add a title above the histogram.
    text.draw_text(
        canvas,
        "Histogram",
        vizlib::Point{right_area.x, 25},
        18.0,
        black
    );

    // Step 11: Add an annotation to the first scatter point.
    vizlib::Annotations annotations;

    const vizlib::Point first_point =
        scatter_axes.dataToPixel(x.front(), y.front());

    annotations.add_text(
        "Sample point",
        vizlib::Point{
            first_point.x + 8,
            first_point.y - 8
        },
        12.0,
        black
    );

    annotations.draw(canvas, text);

    // Step 12: Add a legend to the scatter plot.
    vizlib::Legend legend;

    legend.add_entry("Iris samples", blue);

    legend.set_position(
        vizlib::Point{
            left_area.x + 10,
            left_area.y + 10
        }
    );

    legend.draw(canvas, text, 12.0);

    // Step 13: Draw figure labels and tick labels for both axes.
    figure.drawLabels(canvas, text);

    // Step 14: Save the completed figure as a PNG.
    const char* path = "data/output/pipeline.png";

    if (!vizlib::save_png(canvas, path))
    {
        std::cerr << "Could not save " << path << "\n";
        return 1;
    }

    std::cout << "Saved " << path << "\n";

    return 0;
}
