//This header file declares the class used to convert raw numbers into plot coordinates for an ECDF graph.
#pragma once //to prevent the header file from being included into sourcecode multiple times.
#include <vector>

#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"
#include "vizlib/concepts.hpp"
#include "vizlib/geometry.hpp" // the shared Point type (x, y), the same one Canvas and Axes use
#include "vizlib/plot.hpp"

namespace vizlib{

    // how an ECDF step line looks
    struct EcdfStyle {
        Color color{31, 119, 180, 255};   // blue, the same default colour as the other plots
        int thickness = 2;                // line width in pixels
    };

    // An ECDF plot: for each value x, the share of the data that is <= x.
    //
    // It is a Plot, like ScatterPlot and HistogramPlot, so it can be drawn into
    // any Axes, or put in a JointPlot panel. The curve is drawn as steps: it
    // stays flat between data values and jumps up by 1/n at each one.
    //
    // bounds(): x runs from the smallest to the largest value plus 5% padding,
    // y from 0 to 1.05, so the top step does not touch the border.
    //
    // NaN values are left out. A plot made with data throws
    // std::invalid_argument if the data is empty or only NaN. The default
    // constructor makes an empty plot, used to call compute_coordinates().
    class ECDFPlot : public Plot {
        public:
        ECDFPlot()= default;

        // keeps the ECDF coordinates of `data`, ready to draw
        explicit ECDFPlot(const std::vector<double>& data, EcdfStyle style = {});

        // any other number type (int, float, ...): converted once to double
        template <Numeric T>
        explicit ECDFPlot(const std::vector<T>& data, EcdfStyle style = {})
            : ECDFPlot(to_doubles(data), style) {}

        //class calculating the ecdf coordinates: sorted x, and y = (i + 1) / n.
        //gives the original  variable to the function, promising not to make any changes to it.
        std::vector<Point> compute_coordinates(const std::vector<double> & data) const;

        // the coordinates this plot draws (empty for a default-constructed plot)
        const std::vector<Point>& points() const;

        DataBounds bounds() const override;
        void draw(Canvas& canvas, const Axes& axes) const override;

        private:
        std::vector<Point> points_;
        EcdfStyle style_;
    };

    // how a rug plot looks
    struct RugStyle {
        Color color{31, 119, 180, 255};
        double height = 10.0;   // length of each mark in pixels
        int thickness = 1;
    };

    // A rug plot: one short vertical mark at the bottom of the plot area for
    // every value, so you can see where the data actually lies. Usually drawn
    // under an ECDF or a histogram.
    //
    // NaN values are left out. Throws std::invalid_argument if the data is
    // empty or only NaN, or if height or thickness is not positive.
    class RugPlot : public Plot {
        public:
        explicit RugPlot(const std::vector<double>& data, RugStyle style = {});

        template <Numeric T>
        explicit RugPlot(const std::vector<T>& data, RugStyle style = {})
            : RugPlot(to_doubles(data), style) {}

        DataBounds bounds() const override;
        void draw(Canvas& canvas, const Axes& axes) const override;

        private:
        std::vector<double> values_;
        RugStyle style_;
    };
}
