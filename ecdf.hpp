//This header file acts as a blueprint that defines a point as a pair of points x and y, and declares the function used to convert raw numbers into plot coordinates for an ECDF graph.
#pragma once //to prevent the header file from being included into sourcecode multiple times.
# include <vector>
namespace viz{
    //structure representing a single x, y coorinate
    struct Point{
    double x;
    double y;};

    //class calculating the ecdf coordinates
    class ECDFPlot{
        public:
        ECDFPlot()= default;
        std::vector<Point> compute_coordinates(const std::vector<double> & data);
        //gives the original  variable to the function, promising not to make any changes to it.
    };
}
