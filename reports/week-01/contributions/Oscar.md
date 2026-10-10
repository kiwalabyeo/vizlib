# Joint Plots, Scatter Plots and Histograms

**Member 4:** Kiwalabye Oscar Muwanguzi - 25/U/08733/PS
**Module:** Joint plots (scatter, histogram, joint plot)

## 1. What my module does

A joint plot shows two variables at once. The centre is a scatter plot of x against y. Above it is a histogram of x alone, and to the right is a histogram of y alone, lying on its side. One picture shows both the relationship between the two variables and the distribution of each one. Seaborn's jointplot is the reference design.

My module builds three things, each depending on the one before:

- Scatter plot: one dot per (x, y) pair.
- Histogram: data split into bins, one bar per bin, bar height equal to the bin's count.
- Joint plot: the scatter and two histograms combined in a three-panel layout.

## 2. How it works

### Scatter plot

- For each (x, y) pair, map x through the Axes' x-scale and y through its y-scale to get a pixel position, then draw a filled circle there.
- The axis range comes from the minimum and maximum of the data, plus about 5% padding so no dot sits on the border.
- Edge cases:
  - x and y of different lengths: throw `std::invalid_argument`.
  - Empty data: throw `std::invalid_argument`.
  - A NaN in either value: skip that point.
- Overplotting: with many points the dots merge into a blob. The fix is semi-transparent dots, which needs alpha blending in the Canvas.

### Histogram

- Bin edges and counts come from the stats module. My code draws one rectangle per bin, from edge i to edge i+1, with height equal to the count.
- Common rules for choosing the number of bins:
  - Sturges: k = ceil(log2 n) + 1. Simple, but gives too few bins for large datasets.
  - Freedman–Diaconis: bin width = 2 × IQR × n^(-1/3). Robust to outliers. Planned as the default.
- Density mode: bar height = count / (n × bin width), so the total area of the bars is 1. Needed when a KDE curve is drawn over the bars, so both use the same scale.
- Orientation: the right-hand marginal of a joint plot is a horizontal histogram, with bars growing sideways and bins along the y-axis. Orientation is built into the histogram from the start.

### KDE curve (optional marginal)

- A smooth estimate of the distribution: f(x) = (1 / nh) × Σ K((x − xi) / h), where K is the Gaussian kernel.
- h is the bandwidth. Silverman's rule of thumb: h = 0.9 × min(σ, IQR / 1.34) × n^(-1/5).
- The curve is evaluated at about 200 evenly spaced points across the data range and drawn as connected line segments.
- The stats module (Member 2) computes the KDE values; my module only draws the curve.

### Joint plot layout

- One Figure containing three Axes:
  - Main panel: bottom left, the largest.
  - Top marginal: above the main panel, same width.
  - Right marginal: to the right of the main panel, same height.
- Seaborn's default size ratio is about 5 to 1 (main to marginal), with a small gap between panels.
- Shared scales: the top histogram must use exactly the same x-range and x-scale as the main panel, or its bars will not line up with the dots below. The same applies to the right histogram and the y-axis. The joint plot therefore computes the ranges once and passes them to all three panels.
- The marginal panels hide their count axis, since the shape of the distribution matters there, not the exact numbers.

### Out of scope (possible extensions)

- Hexbin and 2D KDE centre panels, which seaborn also offers. To be added only if time allows.

## 3. Libraries and tools

No external libraries. The module is built only on the team's own modules and the C++ standard library:

- `std::vector` for data
- `<algorithm>` for `std::minmax_element`
- `<cmath>` for `std::isnan` and `std::ceil`

## 4. What I need from others

- **Member 1 (data loading):** a column's values as a `std::vector<double>`, with missing values removed or marked as NaN.
- **Member 2 (statistics):** histogram bin edges and counts, KDE values, min, max and IQR.
- **Member 5 (axes and layout):** a Figure that can hold several Axes at custom positions and sizes, Axes whose x or y range can be set explicitly so panels can share it, and a way to hide one axis's ticks.
- **Member 6 (rendering):** `fill_circle`, `fill_rect`, `draw_line`, and alpha blending for semi-transparent dots.
- **Member 8 (labels):** titles and axis labels.

The most critical dependency is Member 5's multi-Axes layout. The joint plot cannot be built without it.

## 5. What others need from me

- **Member 7 (integration):** the full pipeline example will call the joint plot.
- **Member 3 (ECDF and rug plots):** may reuse the scatter and histogram patterns for drawing on Axes.
- **Library users:** the joint plot is one of the main features of the library.

## 6. Planned interface

To be agreed with the group in the interfaces PR (due Wednesday 7 October):

```cpp
// scatter.hpp
void scatter(Axes& ax, const std::vector<double>& x,
             const std::vector<double>& y, const ScatterStyle& style = {});

// histogram.hpp
enum class Orientation { Vertical, Horizontal };
void histogram(Axes& ax, const std::vector<double>& data,
               const HistogramStyle& style = {});   // bins, density, orientation, colour

// jointplot.hpp
Figure jointplot(const std::vector<double>& x, const std::vector<double>& y,
                 const JointPlotOptions& opts = {});  // size, ratio, marginal kind (bars or KDE)
```

Style and Options structs keep the function signatures short, and allow new options to be added later without breaking existing code.

## 7. Testing plan

**Scatter**
- A known data point lands on the expected pixel, and that pixel has the dot's colour.
- x and y of different lengths throw `std::invalid_argument`.
- Empty data throws `std::invalid_argument`.
- Points containing NaN are skipped.

**Histogram**
- The counts across all bars add up to n.
- The tallest bar matches the largest bin.
- In density mode, the total bar area is approximately 1.
- Horizontal orientation draws bars sideways.

**Joint plot**
- The figure contains three Axes.
- The top marginal's x-range equals the main panel's x-range.
- The right marginal's y-range equals the main panel's y-range.

## 8. Roadmap

- **Week 2 (5 to 11 October):** headers in the interfaces PR by Wednesday. Scatter plot drawing on the Canvas, tested with hand-picked points.
- **Week 3 (12 to 18 October):** histogram in both orientations, with tests. Scatter plot working on real CSV data.
- **Week 4 (19 to 25 October):** joint plot layout with shared scales, then the KDE marginal option.
- **Week 5 (26 to 31 October):** `jointplot_example.cpp`, `docs/jointplot.md`, edge case tests and polish.

## 9. Sources

- Seaborn documentation: `seaborn.jointplot`
- Wikipedia: "Kernel density estimation"
- Wikipedia: "Freedman–Diaconis rule"
- Claus O. Wilke, *Fundamentals of Data Visualization* (free online), chapters on distributions and scatter plots
- B. W. Silverman, *Density Estimation for Statistics and Data Analysis* (1986)

## AI Use

- **Tool:** Claude
- **Purpose:** Explained the structure of joint plots, histogram binning rules, KDE and shared-axis layout, and drafted this research file, which I reviewed and edited.
- **Reason:** To understand the module quickly and catch up after missing the research deadline.
