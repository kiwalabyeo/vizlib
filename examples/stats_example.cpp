// Shows the statistics module (Member 2): a summary of iris petal length, the
// two binning rules through the strategy base class, and the KDE curve drawn
// on its own, with axes. Saves data/output/stats_example.png.
// Run from the project root: ./build/examples/stats_example

#include <vizlib/axes.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/csv_reader.hpp>
#include <vizlib/histogram.hpp>
#include <vizlib/kde.hpp>
#include <vizlib/kde_plot.hpp>
#include <vizlib/stats.hpp>
#include <vizlib/text.hpp>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    try
    {
        const vizlib::DataFrame iris = vizlib::read_csv("data/input/iris.csv");
        const std::vector<double>& petal = iris["petal_length"].values();

        // 1. The summary
        const vizlib::summaryStatistics stats(petal);
        std::cout << "Iris petal length (" << petal.size() << " flowers)\n"
                  << "  mean " << stats.mean() << ", median " << stats.median()
                  << ", standard deviation " << stats.standard_deviation() << "\n"
                  << "  range " << stats.range() << ", quartiles " << stats.quantile(0.25)
                  << " and " << stats.quantile(0.75) << ", IQR " << stats.interquartile_range() << "\n";

        const vizlib::summaryStatistics small({1, 1, 2, 2, 3});
        std::cout << "  mode of {1, 1, 2, 2, 3}:";
        for (double m : small.mode())
            std::cout << " " << m;
        std::cout << " (two modes, which is why mode() returns a vector)\n";

        // 2. The strategy pattern: the same call runs whichever rule the object is
        std::vector<std::unique_ptr<vizlib::binningStrategy>> rules;
        rules.push_back(std::make_unique<vizlib::SturgesBinning>(petal));
        rules.push_back(std::make_unique<vizlib::FreedmanDiaconisBinning>(petal));
        const char* names[] = {"Sturges", "Freedman-Diaconis"};
        for (std::size_t i = 0; i < rules.size(); ++i)
        {
            const std::vector<double> edges = rules[i]->histogram_bin_edges();
            const std::vector<std::size_t> counts = vizlib::histogram_counts(petal, edges);
            std::cout << "  " << names[i] << ": " << counts.size() << " bins, counts";
            for (std::size_t c : counts)
                std::cout << " " << c;
            std::cout << "\n";
        }

        // 3. The KDE curve, drawn on its own with axes
        const vizlib::KdeResult curve = vizlib::kde(petal);
        const double peak = *std::max_element(curve.density.begin(), curve.density.end());
        std::cout << "  KDE: bandwidth h = " << curve.bandwidth << ", highest density " << peak << "\n";

        const vizlib::Color white{255, 255, 255, 255};
        const vizlib::Color dark{60, 60, 60, 255};
        vizlib::Canvas canvas(520, 340, white);
        const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");

        const vizlib::KdePlot plot(curve);
        const vizlib::DataBounds b = plot.bounds();
        vizlib::Axes axes;
        axes.setPlotArea({60, 40, 430, 240});
        axes.setDataRange(b.x_min, b.x_max, b.y_min, b.y_max);
        plot.draw(canvas, axes);
        axes.drawAxes(canvas, dark);
        axes.drawTickLabels(canvas, text);
        text.draw_text(canvas, "KDE of iris petal length", {60, 24}, 16, dark);
        text.draw_text(canvas, "Petal length (cm)", {220, 328}, 13, dark);

        const char* path = "data/output/stats_example.png";
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
