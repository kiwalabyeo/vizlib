//This header file declares the class used to convert raw numbers into plot coordinates for an ECDF graph.
#pragma once //to prevent the header file from being included into sourcecode multiple times.
# include <vector>
#include "vizlib/geometry.hpp" // the shared Point type (x, y), the same one Canvas and Axes use
namespace vizlib{
    //class calculating the ecdf coordinates
    class ECDFPlot{
        public:
        ECDFPlot()= default;
        std::vector<Point> compute_coordinates(const std::vector<double> & data);
        //gives the original  variable to the function, promising not to make any changes to it.
    };
}
