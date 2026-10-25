# Week 04 Report: Member 4, Joint Plot

**Name:** Kiwalabye Oscar Muwanguzi 25/U/08733/PS
**Task area:** Joint plots (this week: the joint plot, and finishing the scatter plot on real data)
**Week:** 4 (finalising week)
**Branch / pull request:** `feature/joint-plot`, PR #TBD

---

## 1. What I did this week

My goals were:

1. Finish the Week 3 goal that was blocked: the scatter plot on real CSV data (`iris.csv`).
2. Move the scatter plot from a `Rect` to Member 5's `Axes`, like the histogram.
3. Build the joint plot: a scatter plot with the distribution of x on top and of y on the right, sharing scales, with either histogram or KDE margins.
4. Give the project real inheritance and polymorphism, which I proposed in my Week 3 report.

All four are done. I did the following:

- Added a `Plot` base class. `ScatterPlot`, `HistogramPlot` and the new `KdePlot` derive from it.
- Added `JointPlot`, which holds three `Plot`s and draws them in three panels with shared scales.
- `scatter()` can now draw into an `Axes`. The old `Rect` version still works, so nobody's code breaks.
- Switched `scatter_example.cpp` to iris, read with Member 1's `read_csv()`, and added `joint_plot_example.cpp`. Member 1's `iris.csv` had 14 sample rows; I extended it to the full 150-flower data set (the original 14 rows are unchanged).
- Added `auto_bin_edges()`, so histograms finally use Member 2's Freedman–Diaconis rule.
- Added Member 1's `csv_reader.cpp` and `dataframe.cpp` and Member 2's `stats.cpp` to the build. They were on `main` but not compiled. I also removed duplicate entries from the CMake source lists.
- Wrote 13 new test cases: 10 in `tests/test_joint_plot.cpp` and 3 in `tests/test_histogram.cpp`.

## 2. Files I created or changed

| File                                 | Purpose                                                              |
| ------------------------------------ | -------------------------------------------------------------------- |
| `include/vizlib/plot.hpp`            | `Plot` base class and `DataBounds`.                                  |
| `include/vizlib/scatter.hpp`, `src/scatter.cpp` | `ScatterPlot` class; `scatter()` now also takes an `Axes`. |
| `include/vizlib/histogram.hpp`, `src/histogram.cpp` | `HistogramPlot` class and `auto_bin_edges()`; `histogram()` now uses `HistogramPlot`. |
| `include/vizlib/kde_plot.hpp`, `src/kde_plot.cpp` | `KdePlot`: draws KDE values as a curve.                  |
| `include/vizlib/joint_plot.hpp`, `src/joint_plot.cpp` | `JointPlot`, `JointLayout`, `JointAxes`.             |
| `tests/test_joint_plot.cpp`, `tests/test_histogram.cpp` | Tests for the classes above and `auto_bin_edges()`. |
| `examples/example_utils.hpp`         | Turns a DataFrame column into numbers for plotting.                 |
| `examples/scatter_example.cpp`       | Now plots iris instead of hand-picked points.                        |
| `examples/joint_plot_example.cpp`    | Iris joint plots with histogram and with KDE margins.                |
| `data/input/iris.csv`                | Extended from 14 sample rows to the full 150-row iris data set.      |
| `CMakeLists.txt`, `tests/CMakeLists.txt`, `examples/CMakeLists.txt` | Build the new files.                  |

## 3. How the joint plot works

```cpp
DataFrame iris = read_csv("data/input/iris.csv");
JointPlot joint = JointPlot::with_histograms(x, y);   // Freedman–Diaconis bins
joint.draw(canvas, {30, 50, 400, 380});
save_png(canvas, "data/output/joint_plot_example.png");
```

1. **Layout.** The area is split into three panels. The top margin takes 20% of the height and the right margin 20% of the width (`JointLayout::margin_ratio`), with a 6 px gap. The centre panel gets the rest.
2. **Shared scales.** Each panel's `bounds()` says what data range it needs. The centre and top panels get the same x range: the union of both, so it is wide enough for the scatter points and the histogram bins. The centre and right panels share a y range the same way. A given x value therefore lands in the same pixel column in the centre and the top margin, so the bars sit exactly above the points they count.
3. **Drawing.** `JointPlot` calls `draw()` on each panel. It does not know which kind of plot each one is.

The KDE values are passed in by the caller, because the team agreed in Week 1 that the stats module computes KDE and the plot only draws it. The example computes them with a small temporary function until Member 2's KDE is merged.

## 4. Inheritance, polymorphism and encapsulation

### Inheritance

```
                 Plot  (abstract base class)
        + bounds() const : DataBounds        = 0   (pure virtual)
        + draw(Canvas&, const Axes&) const   = 0   (pure virtual)
        + ~Plot()                                  (virtual)
            ▲                ▲                ▲
            │ public         │ public         │ public
     ScatterPlot       HistogramPlot        KdePlot
```

`ScatterPlot`, `HistogramPlot` and `KdePlot` inherit publicly from `Plot`: each one *is a* plot. `Plot` only describes what every plot can do (say its data range, and draw itself into an `Axes`). Each derived class says how. `Plot` has pure virtual functions, so it is abstract, and a plain `Plot` cannot be created. Its destructor is virtual, so deleting a `ScatterPlot` through a `Plot` pointer runs the right destructor.

`JointPlot` does not inherit from `Plot`. It *has* three plots (composition), held as `std::unique_ptr<Plot>`, so it owns them and frees them automatically.

### Polymorphism

```cpp
// JointPlot::draw
center_->draw(canvas, axes.center);
top_->draw(canvas, axes.top);
right_->draw(canvas, axes.right);
```

These are virtual calls. The same line runs `ScatterPlot::draw`, `HistogramPlot::draw` or `KdePlot::draw` depending on the real type of the object, which is decided at run time. That is why the example can swap histogram margins for KDE margins without changing any `JointPlot` code. Member 3's ECDF could become a fourth `Plot` and work in a joint plot straight away.

A test proves this: it defines a `FakePlot` that derives from `Plot` inside the test file, and checks that `JointPlot` calls its `bounds()` and `draw()` and gives it the right panel area.

### Encapsulation

- **Data is private.** `ScatterPlot` keeps `x_`, `y_`, `style_` and `bounds_` private. `HistogramPlot` keeps `edges_`, `counts_` and `style_` private, and `KdePlot` keeps `grid_`, `density_` and `style_` private. `JointPlot` keeps its three panels private.
- **Checked in the constructor.** Each constructor throws `std::invalid_argument` on bad input: different lengths, empty data, all NaN, bad edges, negative density. Because the data is private and cannot be changed afterwards, a plot object that exists is always valid and can always be drawn. `draw()` never needs to check again.
- **Work done once.** `HistogramPlot` counts the values in its constructor. `counts()` gives read-only access (a `const` reference), so other code can look at the counts but not change them.
- **Hidden helpers.** `draw_dot()`, `fill_box()`, `check_edges()` and `draw_segment()` are in unnamed namespaces in their `.cpp` files, so no other file can use them.
- **Style structs stay public.** `ScatterStyle`, `HistogramStyle`, `KdeStyle` and `JointLayout` are structs on purpose. They hold options that are all valid, so there is nothing to protect.

### Class layout (for the paper drawing)

```
DataBounds (struct)          Plot «abstract»
+ x_min, x_max : double      + bounds() : DataBounds  {virtual = 0}
+ y_min, y_max : double      + draw(Canvas&, const Axes&) {virtual = 0}
                                  ▲ (inheritance, hollow arrow)
         ┌────────────────────────┼─────────────────────────┐
ScatterPlot                 HistogramPlot                 KdePlot
− x_, y_ : vector<double>   − edges_ : vector<double>     − grid_, density_ : vector<double>
− style_ : ScatterStyle     − counts_ : vector<size_t>    − style_ : KdeStyle
− bounds_ : DataBounds      − style_ : HistogramStyle
+ ScatterPlot(x, y, style)  + HistogramPlot(data, edges, style)  + KdePlot(grid, density, style)
+ bounds() {override}       + counts() : const vector&    + bounds() {override}
+ draw()   {override}       + bounds() {override}         + draw()   {override}
                            + draw()   {override}

JointPlot                                         JointAxes (struct)
− center_, top_, right_ : unique_ptr<Plot> ◆───── 3 Plots (composition, filled diamond)
+ JointPlot(center, top, right)                   + center, top, right : Axes
+ with_histograms(x, y, x_edges, y_edges) {static}
+ layout(Rect, JointLayout) : JointAxes
+ draw(Canvas&, Rect, JointLayout)
```

`−` is private and `+` is public.

## 5. How I tested it

All 42 test cases pass: 29 existing ones and 13 new ones. The new tests check that:

- `ScatterPlot` bounds are the data range plus 5%, ignoring NaN points.
- `scatter()` through an `Axes` draws exactly the same pixels as the old `Rect` version.
- `HistogramPlot` and `KdePlot` bounds swap x and y in horizontal orientation, and a horizontal KDE curve is drawn sideways.
- `KdePlot` rejects bad input and draws its line through the right pixels.
- `JointPlot` works with any `Plot` (the `FakePlot` test), lays out the panels correctly, and the margins share the centre panel's scales.
- `with_histograms()` draws into all three panels and leaves the empty corner empty.
- Null panels, bad layouts and bad data all throw `std::invalid_argument`.
- `auto_bin_edges()` gives edges from exactly the min to the max, in any input order, never loses a value, and handles an IQR of 0, equal values and NaN.

I also ran all tests and examples with AddressSanitizer and UndefinedBehaviorSanitizer, with no errors. I broke the code on purpose 5 ways: margins not sharing x, margins not sharing y, the right panel never drawn, KDE orientation ignored, and negative density allowed. The first run missed the KDE orientation bug, so I added a test for it. Now all 5 are caught.

## 6. Problems I faced and how I solved them

- **The CSV reader arrived but was not built (Member 1).** `read_csv()` and `DataFrame` were on `main` but not in `CMakeLists.txt`, so nothing could use them. I added them, and the examples now read iris with `read_csv()`. The DataFrame stores each cell as a number or a string, so a small helper in `examples/example_utils.hpp` turns a column into numbers (text becomes NaN, which the plots skip).
- **No KDE yet (Member 2).** `KdePlot` draws KDE values passed in by the caller. The example computes them with a temporary Gaussian KDE until Member 2's is merged.
- **Freedman–Diaconis bin edges (Member 2).** Member 2's updated `histogram_bin_edges()` now sorts the data and includes the max. Two things still needed handling, so `auto_bin_edges()` takes only the *number* of bins from it: its last edge can overshoot the max (13.5 for data 1 to 10), which would stretch the joint plot's shared scale, so the edges are spread evenly from min to max like numpy; and with an IQR of 0 it returns a single edge, so Sturges' rule is used instead. `stats.cpp` was also not being built.
- **No `draw_line`, `fill_rect` or `fill_circle` yet (Member 6).** `KdePlot` uses a private `draw_segment()` helper, like `draw_dot()` and `fill_box()`. All three are marked to be replaced.
- **`histogram()` set the axes range itself,** which would have broken shared scales. I split each plot into `bounds()` and `draw()`, so `JointPlot` can set a shared range first. `histogram()` and `scatter()` still set the range themselves, so the earlier tests did not change.

## 7. What is still pending

- Switch to Member 2's KDE and Member 6's drawing functions once each is merged.
- Axis lines, ticks and labels on the joint plot, once Member 5's tick code and drawing are merged.
- PR review (three approvals).

## AI Use

- **Tool:** Claude (Claude Code)
- **Purpose:** Designed and drafted the `Plot` class hierarchy, joint plot, tests and examples, ran the tests and sanitizers, and drafted this report.
- **Reason:** To finish the joint plot in the final week despite several missing dependencies, while keeping it tested.
