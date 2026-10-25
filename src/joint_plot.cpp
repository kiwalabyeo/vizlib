#include "vizlib/joint_plot.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace vizlib {

JointPlot::JointPlot(std::unique_ptr<Plot> center, std::unique_ptr<Plot> top,
                     std::unique_ptr<Plot> right)
    : center_(std::move(center)), top_(std::move(top)), right_(std::move(right)) {
    if (!center_ || !top_ || !right_) {
        throw std::invalid_argument("joint plot: every panel must be set.");
    }
}

JointPlot JointPlot::with_histograms(const std::vector<double>& x, const std::vector<double>& y,
                                     const std::vector<double>& x_edges,
                                     const std::vector<double>& y_edges,
                                     const ScatterStyle& style) {
    HistogramStyle top_style;
    top_style.color = style.color;
    HistogramStyle right_style = top_style;
    right_style.orientation = Orientation::Horizontal;

    return JointPlot(std::make_unique<ScatterPlot>(x, y, style),
                     std::make_unique<HistogramPlot>(x, x_edges, top_style),
                     std::make_unique<HistogramPlot>(y, y_edges, right_style));
}

JointAxes JointPlot::layout(Rect area, const JointLayout& layout) const {
    if (!(layout.margin_ratio > 0 && layout.margin_ratio < 1) || !(layout.gap >= 0)) {
        throw std::invalid_argument("joint plot: margin_ratio must be between 0 and 1, gap not negative.");
    }

    // margin sizes, then the centre panel takes what is left
    const double top_height = area.height * layout.margin_ratio;
    const double right_width = area.width * layout.margin_ratio;
    const double main_width = area.width - right_width - layout.gap;
    const double main_height = area.height - top_height - layout.gap;
    const double main_y = area.y + top_height + layout.gap;
    if (main_width <= 0 || main_height <= 0) {
        throw std::invalid_argument("joint plot: area too small for the gap and margins.");
    }

    // shared scales: centre and top use the same x range, centre and right
    // the same y range, wide enough for both plots
    const DataBounds c = center_->bounds();
    const DataBounds t = top_->bounds();
    const DataBounds r = right_->bounds();
    const double x_min = std::min(c.x_min, t.x_min);
    const double x_max = std::max(c.x_max, t.x_max);
    const double y_min = std::min(c.y_min, r.y_min);
    const double y_max = std::max(c.y_max, r.y_max);

    JointAxes axes;
    axes.center.setPlotArea({area.x, main_y, main_width, main_height});
    axes.center.setDataRange(x_min, x_max, y_min, y_max);

    axes.top.setPlotArea({area.x, area.y, main_width, top_height});
    axes.top.setDataRange(x_min, x_max, t.y_min, t.y_max);

    axes.right.setPlotArea({area.x + main_width + layout.gap, main_y, right_width, main_height});
    axes.right.setDataRange(r.x_min, r.x_max, y_min, y_max);
    return axes;
}

void JointPlot::draw(Canvas& canvas, Rect area, const JointLayout& layout) const {
    const JointAxes axes = this->layout(area, layout);
    // virtual calls: each panel draws itself, whatever kind of plot it is
    center_->draw(canvas, axes.center);
    top_->draw(canvas, axes.top);
    right_->draw(canvas, axes.right);
}

}  // end of namespace vizlib
