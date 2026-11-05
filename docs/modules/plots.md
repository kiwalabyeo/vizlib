# Plots: scatter, histogram, KDE curve and joint plot

- **Owner:** Kiwalabye Oscar Muwanguzi (Member 4)
- **Headers:** `include/vizlib/plot.hpp`, `scatter.hpp`, `histogram.hpp`, `kde_plot.hpp`, `joint_plot.hpp`, `concepts.hpp`
- **Sources:** `src/scatter.cpp`, `src/histogram.cpp`, `src/kde_plot.cpp`, `src/joint_plot.cpp`
- **Tests:** `tests/test_scatter.cpp`, `test_histogram.cpp`, `test_joint_plot.cpp` and `test_plot_templates.cpp` (42 test cases; the template ones run once for each of 7 number types)
- **Examples:** `scatter_example`, `histogram_example`, `joint_plot_example`, and the plot part of `stats_example`

## 1. What it does

This module turns numbers into plots on a `Canvas`:

- **`ScatterPlot`:** one dot per (x, y) pair.
- **`HistogramPlot`:** counts how many values fall in each bin and draws a bar per bin, upright or sideways.
- **`KdePlot`:** draws the smooth KDE curve that Member 2's `kde()` works out.
- **`JointPlot`:** a scatter in the middle, the spread of x in a panel on top, and the spread of y in a panel on the right. The margins can be histograms or KDE curves.

```cpp
#include <vizlib/joint_plot.hpp>

vizlib::Canvas canvas(900, 460, vizlib::Color{255, 255, 255, 255});
const vizlib::JointPlot plot = vizlib::JointPlot::with_kde(x, y);   // or with_histograms(x, y)
plot.draw(canvas, {55, 50, 390, 380});
```

## 2. How it works

**Every plot is a `Plot`.** `Plot` is an abstract base class with two pure virtual functions:

1. `bounds()` says what data range the plot needs.
2. The caller sets the `Axes` to that range, or to a wider one shared with other panels.
3. `draw(canvas, axes)` draws into those axes. Every point goes through `axes.dataToPixel`, so the plots never deal with pixels or scales themselves.

**Scatter.** `bounds()` is the smallest and largest x and y, plus 5% padding on each side, so no dot sits on the border. `draw()` puts a filled circle (Member 6's `fill_circle`) at every point. Points with a NaN in x or y are skipped.

**Histogram.** `histogram_counts(data, edges)` puts each value in its bin with `std::upper_bound` on the sorted edges. That is O(log b) per value for b bins. Bin i covers [edges[i], edges[i+1]), and the last bin also includes its right edge, the same as numpy. Values outside the edges and NaN values are not counted. `auto_bin_edges(data)` takes the number of bins from Member 2's binning rules and spreads the edges evenly from the smallest to the largest value. `draw()` fills one rectangle per bin that is not empty.

**KDE curve.** The curve is straight lines joining each (grid[i], density[i]) point. `bounds()` runs from the first to the last grid point, and from 0 to the highest density plus 5%.

**Joint plot.** `layout(area)` splits the area into three panels: the top and right margins take 20% each, with a 6-pixel gap. The **x range is shared** by the centre and the top panel, and the **y range** by the centre and the right panel, so a given x value lands in the same pixel column in both. `draw()` then calls `draw()` on the three panels.

## 3. Why it is written this way

- **Inheritance and runtime polymorphism.** `JointPlot` holds three `std::unique_ptr<Plot>` and calls `draw()` on each one without knowing what kind of plot it is. That is why histogram margins and KDE margins use exactly the same `JointPlot` code, and why Crystal's `ECDFPlot` could be used as a panel with no change. `Plot` has a virtual destructor, so deleting a `ScatterPlot` through a `Plot` pointer runs the right destructor.
- **`unique_ptr`, not `shared_ptr`.** The joint plot is the only owner of its panels. As a result, a `JointPlot` can be moved but not copied.
- **Templates only where the type is stored.**
  - `ScatterPlot<TX, TY>` is a class template, because it keeps its data in its own type. x and y can even have different types, such as `int` years and `double` prices.
  - `HistogramPlot` only keeps bin edges and counts, which never depend on the data type. So only its constructor is a template: it counts the data and throws it away.
  - The helpers `scatter`, `histogram`, `histogram_counts`, `auto_bin_edges`, `with_histograms` and `with_kde` are function templates.
- **The `Numeric` concept (C++20).** It accepts integers and floating-point types, but not `bool` or `char`. A wrong type gives one clear compile error instead of a page of template errors.
- **Encapsulation.** The data is private, and each constructor checks its input. It throws `std::invalid_argument` if x and y have different lengths, if the data is empty or only NaN, or if the edges are not increasing. A plot that exists can always be drawn.
- **`const`.** `bounds()` and `draw()` only read the plot, so they are `const` and work on a `const` plot.
- **Double versions next to the templates.** A brace list like `with_histograms({1, 2}, {3, 4})` has no type that a template can deduce, so each template also has a plain `double` version.

## 4. How it is tested

- **Normal use:**
  - the bounds are the data range plus 5%
  - `histogram_counts({0, 1, 2}, {0, 1, 2})` gives `{1, 2}`, because the last bin includes its right edge
  - the joint plot draws something in all three panels
- **Edge cases:**
  - NaN values are skipped
  - values outside the edges are not counted
  - the margins share the centre panel's scales, checked pixel by pixel
  - a custom `JointLayout` is followed
- **Bad input:** different lengths, empty data, data that is only NaN, edges that are not increasing, a null panel and a bad layout all throw.
- **Templates:** each template test runs once for 7 number types, and must give exactly the same result as `double`.
- **Polymorphism:** the `FakePlot` test defines a new plot type inside the test file. `JointPlot` draws it with no change.

## 5. Limitations

- `scatter()` and `histogram()` set the axes range from the data plus 5% padding. On a log axis that padding can go below 0, which a log scale refuses. The fix is to set the range and the log scale first, then call the plot's `draw()` (see `log_axis_example`).
- The margin panels of a joint plot have no tick numbers, the same as seaborn's joint plots.
- `JointPlot` cannot be copied, only moved.

## 6. Who changed it

- **Oscar:** scatter (#51), histogram (#57), `Plot` and `JointPlot` (#73), C++20 and the templates (#92, #93), the KDE joint plot (#115), and the axes on the examples (#129).
- **End-of-project fixes, done with Claude Code:** `auto_bin_edges` now refuses infinity (`docs/bug-report.md`, bug 6), and the joint-layout test was added (#131).
- **Members 2 and 6:** `auto_bin_edges` uses Member 2's binning rules, and the dots and bars are drawn with Member 6's Canvas.
