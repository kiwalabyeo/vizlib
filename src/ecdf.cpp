#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include <vizlib/ecdf.hpp>

namespace vizlib{

namespace {

// the values that are not NaN (NaN cannot be sorted or placed on an axis)
std::vector<double> without_nan(const std::vector<double>& data) {
    std::vector<double> values;
    for (double v : data) {
        if (!std::isnan(v)) {
            values.push_back(v);
        }
    }
    return values;
}

// x range of the data plus 5% padding on each side; a range of width 0 gets
// a width of 1, so the axes never divide by zero
void padded_range(double lo, double hi, double& out_lo, double& out_hi) {
    double span = hi - lo;
    if (span == 0) {
        lo -= 0.5;
        hi += 0.5;
        span = 1.0;
    }
    out_lo = lo - 0.05 * span;
    out_hi = hi + 0.05 * span;
}

}  // end of unnamed namespace

    std::vector<Point> ECDFPlot::compute_coordinates(const std::vector<double> &data) const{
    // making a copy of input data to sort, without NaN: std::sort needs values
    // that can be compared, and NaN is not smaller or bigger than anything
    std::vector<double> sorted_data = without_nan(data);
    if (sorted_data.empty()){
        return {};
    }
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

ECDFPlot::ECDFPlot(const std::vector<double>& data, EcdfStyle style)
    : style_(style) {
    if (style_.thickness < 1) {
        throw std::invalid_argument("ecdf: thickness must be at least 1.");
    }
    points_ = compute_coordinates(data);
    if (points_.empty()) {
        throw std::invalid_argument("ecdf: data has no value that is not NaN.");
    }
}

const std::vector<Point>& ECDFPlot::points() const {
    return points_;
}

DataBounds ECDFPlot::bounds() const {
    if (points_.empty()) {
        return DataBounds{};   // empty plot: the default range 0..1
    }
    DataBounds b;
    padded_range(points_.front().x, points_.back().x, b.x_min, b.x_max);
    b.y_min = 0.0;
    b.y_max = 1.05;
    return b;
}

void ECDFPlot::draw(Canvas& canvas, const Axes& axes) const {
    // Steps: start on the x axis under the first value, go up to its y, then
    // for each next value go across (flat) and up. A repeated value makes a
    // flat part of length 0, so its jump is bigger: 2/n for two equal values.
    double previous_y = 0.0;
    for (std::size_t i = 0; i < points_.size(); ++i) {
        const Point p = points_[i];
        if (i > 0) {
            // across, at the previous height, from the previous value to this one
            canvas.draw_line(axes.dataToPixel(points_[i - 1].x, previous_y),
                             axes.dataToPixel(p.x, previous_y),
                             style_.color, style_.thickness);
        }
        // up, at this value
        canvas.draw_line(axes.dataToPixel(p.x, previous_y),
                         axes.dataToPixel(p.x, p.y),
                         style_.color, style_.thickness);
        previous_y = p.y;
    }
}

RugPlot::RugPlot(const std::vector<double>& data, RugStyle style)
    : values_(without_nan(data)), style_(style) {
    if (values_.empty()) {
        throw std::invalid_argument("rug: data has no value that is not NaN.");
    }
    if (!(style_.height > 0) || style_.thickness < 1) {
        throw std::invalid_argument("rug: height and thickness must be positive.");
    }
}

DataBounds RugPlot::bounds() const {
    const auto [lo, hi] = std::minmax_element(values_.begin(), values_.end());
    DataBounds b;
    padded_range(*lo, *hi, b.x_min, b.x_max);
    b.y_min = 0.0;
    b.y_max = 1.0;
    return b;
}

void RugPlot::draw(Canvas& canvas, const Axes& axes) const {
    // the marks stand on the bottom edge of the plot area, whatever its y range
    const Rect area = axes.getPlotArea();
    const double bottom = area.y + area.height;
    // a y value that is valid on this axis (0 is not, on a log axis): the one
    // at the bottom edge. Only the x pixel of each value is needed.
    const double y_at_bottom = axes.pixelToData(area.x, bottom).y;
    for (double v : values_) {
        const double x = axes.dataToPixel(v, y_at_bottom).x;
        canvas.draw_line(Point{x, bottom}, Point{x, bottom - style_.height},
                         style_.color, style_.thickness);
    }
}

}
