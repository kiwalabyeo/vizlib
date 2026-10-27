#pragma once

#include <vector>
#include "vizlib/axes.hpp"
#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"
#include "vizlib/histogram.hpp"
#include "vizlib/plot.hpp"

namespace vizlib {

// how a KDE curve looks
struct KdeStyle {
    Color color{31, 119, 180, 255};
    double thickness = 2.0;                            // line width in pixels
    Orientation orientation = Orientation::Vertical;   // Horizontal: grid on y, density on x
};

// A KDE curve: straight lines joining (grid[i], density[i]).
//
// The stats module computes the KDE values; this class only draws them
// (agreed in Week 1).
//
// bounds(): the grid axis runs from the first to the last grid point, and the
// density axis from 0 to the highest density plus 5% padding.
//
// The constructor throws std::invalid_argument if grid and density have
// different lengths or fewer than 2 points, the grid is not finite and
// strictly increasing, or a density is negative or not finite.
class KdePlot : public Plot {
private:
    std::vector<double> grid_;
    std::vector<double> density_;
    KdeStyle style_;

public:
    KdePlot(std::vector<double> grid, std::vector<double> density, KdeStyle style = {});

    DataBounds bounds() const override;
    void draw(Canvas& canvas, const Axes& axes) const override;
};

}  // end of namespace vizlib
