# include <algorithm>
#include <vizlib/ecdf.hpp>
# include <vector>

namespace vizlib{
    std::vector<Point> ECDFPlot::compute_coordinates(const std::vector<double> &data){
    if (data.empty()){
        return {};
    }
// making a copy of input data to sort
    std::vector<double> sorted_data = data;
    //sort in ascending order
    std::sort (sorted_data.begin(),sorted_data.end());
    //calculate (x,y) coordinates
    std::vector<Point> points;
    const double total_count = static_cast<double>(sorted_data.size());
    for(std::size_t i =0; i<sorted_data.size();++i){
        double x = sorted_data[i];
        double y = static_cast<double>(i+1)/total_count;
        points.push_back(Point{x,y});
    }
    return points;
}
}
