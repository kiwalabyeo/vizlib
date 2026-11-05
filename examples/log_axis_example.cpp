// Shows log axes (Member 5): the same doubling data on a linear y axis and on
// a log y axis, where it becomes a straight line. Saves
// data/output/log_axis_example.png.
// Run from the project root: ./build/examples/log_axis_example
//
// The order matters on a log axis: set the data range first (it must be above
// 0), then switch the axis to ScaleType::Logarithmic.

#include <vizlib/axes.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/figure.hpp>
#include <vizlib/scatter.hpp>
#include <vizlib/text.hpp>

#include <cmath>
#include <exception>
#include <iostream>
#include <vector>

int main()
{
    try
    {
        // a value that doubles every step: 1, 2, 4, ... 1024
        std::vector<double> t, y;
        for (int i = 0; i <= 10; ++i)
        {
            t.push_back(i);
            y.push_back(std::pow(2.0, i));
        }

        const vizlib::Color white{255, 255, 255, 255};
        const vizlib::Color dark{60, 60, 60, 255};
        vizlib::Canvas canvas(800, 380, white);
        vizlib::Figure figure(800, 380);

        // left: linear y axis
        vizlib::Axes& linear = figure.addAxes(vizlib::Rect{0, 20, 400, 340});
        linear.setDataRange(0, 10, 0, 1100);

        // right: log y axis. Range first, then the scale.
        vizlib::Axes& logarithmic = figure.addAxes(vizlib::Rect{400, 20, 400, 340});
        logarithmic.setDataRange(0, 10, 1, 1100);
        logarithmic.setYScale(vizlib::ScaleType::Logarithmic);

        // draw the plot straight into the axes: scatter() would pad the range
        // by 5%, which goes below 0 and is not allowed on a log axis
        vizlib::ScatterStyle style;
        style.radius = 4;
        const vizlib::ScatterPlot plot(t, y, style);
        plot.draw(canvas, linear);
        plot.draw(canvas, logarithmic);

        const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
        figure.drawAxes(canvas);
        for (std::size_t i = 0; i < figure.getAxesCount(); ++i)
            figure.getAxes(i).drawTickLabels(canvas, text);
        text.draw_text(canvas, "Linear y axis: a curve", {60, 36}, 16, dark);
        text.draw_text(canvas, "Log y axis: a straight line", {460, 36}, 16, dark);
        text.draw_text(canvas, "Step", {200, 372}, 13, dark);
        text.draw_text(canvas, "Step", {600, 372}, 13, dark);

        const char* path = "data/output/log_axis_example.png";
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
