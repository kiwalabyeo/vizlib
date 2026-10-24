# Week 03 Report: Member 4, Histogram

**Name:** Kiwalabye Oscar Muwanguzi 25/U/08733/PS
**Task area:** Joint plots (this week: the histogram, and the scatter plot on real data)
**Week:** 12 October to 18 October 2026
**Branch / pull request:** `feature/histogram`, PR #57

---

## 1. What I did this week

My Week 3 goals were:

1. A histogram in both orientations, vertical and horizontal.
2. The scatter plot working on real CSV data (`iris.csv`), not hand-picked points.

Goal 1 is done. It was finished one day late, on 19 October, and merged on 20 October in PR #57. Goal 2 is not done, because the CSV reader is not on `main` yet (see section 6).

For the histogram I did the following:

- Wrote `histogram()`, which draws one bar per bin through Member 5's `Axes`. A setting in `HistogramStyle` chooses vertical bars (growing upwards) or horizontal bars (growing to the right, which the joint plot margins need).
- Wrote `histogram_counts()`, which counts how many values fall in each bin.
- Wrote 8 tests in `tests/test_histogram.cpp` and `examples/histogram_example.cpp`, which draws the same data both ways and saves a PNG.
- Added Member 5's `scale.cpp`, `axes.cpp` and `figure.cpp` to `CMakeLists.txt`. They were on `main` but were not being built.

My scatter plot from Week 2 (PR #51) was also merged this week, on 15 October.

## 2. Files I created or changed

| File                              | Purpose                                                          |
| --------------------------------- | ---------------------------------------------------------------- |
| `include/vizlib/histogram.hpp`    | Declares `Orientation`, `HistogramStyle`, `histogram_counts()` and `histogram()`. |
| `src/histogram.cpp`               | Counts values per bin and draws the bars.                        |
| `tests/test_histogram.cpp`        | Tests for the histogram.                                         |
| `examples/histogram_example.cpp`  | Draws 500 values both ways and saves a PNG.                      |
| `CMakeLists.txt`                  | Adds `scale.cpp`, `axes.cpp`, `figure.cpp` and `histogram.cpp`.  |
| `tests/CMakeLists.txt`            | Adds `test_histogram.cpp`.                                       |
| `examples/CMakeLists.txt`         | Builds the example.                                              |
| `docs/changelogs/Kiwalabye-Oscar.md` | Changelog entry.                                              |

## 3. How the histogram works

```cpp
std::vector<std::size_t> histogram_counts(const std::vector<double>& data,
                                          const std::vector<double>& edges);

void histogram(Canvas& canvas, Axes& axes,
               const std::vector<double>& data, const std::vector<double>& edges,
               const HistogramStyle& style = {});
```

1. **Check the edges.** There must be at least 2, and they must be finite and strictly increasing. Otherwise `std::invalid_argument` is thrown. Empty data, or data that is all NaN, also throws.
2. **Count the values.** Bin `i` covers `[edges[i], edges[i+1])`. The last bin also includes its right edge, so the largest value is never lost (the same rule as numpy). NaN values and values outside the edges are skipped. `std::upper_bound` finds each value's bin.
3. **Set the axes range.** The bin axis runs from the first edge to the last. The count axis runs from 0 to the tallest bar plus 5%, so the tallest bar does not touch the top.
4. **Draw the bars.** For each bin, `Axes::dataToPixel()` turns the two corners of the bar into pixels, and the rectangle between them is filled. In horizontal mode the x and y roles are swapped.

The caller passes the bin edges for now. The plan was to use Member 2's Freedman–Diaconis edges, but `histogram_bin_edges()` cannot be used yet (see section 6). Once it is fixed I will add a version of `histogram()` that works out the edges itself.

## 4. How I tested it

All 27 test cases pass. That is the 19 existing ones and 8 new ones. My tests check that:

- Values land in the right bins, a value on a shared edge goes in the right-hand bin, and a value equal to the last edge is counted.
- NaN values and values outside the edges are not counted.
- Bad edges (one edge, decreasing, repeated, infinite) throw `std::invalid_argument`.
- Vertical bars have the right heights. For example, a count of 1 out of a maximum of 4 is 23.8 px tall on a 100 px area.
- Horizontal bars point to the right, with the lowest bin at the bottom.
- `histogram()` sets the axes data range correctly.

I also ran the tests with AddressSanitizer and UndefinedBehaviorSanitizer, with no errors. To check that the tests can catch mistakes, I broke the code on purpose three ways: the last edge not counted, no 5% headroom, and orientation ignored. The tests failed each time, and passed again once the code was restored.

## 5. Inheritance, polymorphism and encapsulation

### Encapsulation

Encapsulation means a class keeps its data `private` and only lets other code change it through `public` functions that check the change is valid. My module uses it in two ways.

**In the classes I use from other members:**

- **`Canvas` (Member 6)** keeps `width_`, `height_` and `pixels_` private. The only way to change a pixel is the public `set_pixel()`, which ignores positions outside the canvas. My histogram and scatter code can therefore never write outside the pixel array, even when a bar or dot is partly off the canvas.
- **`Axes` (Member 5)** keeps its data range, plot area and two `LinearScale` objects private. `setDataRange()` throws if the range is not finite or min ≥ max, so an `Axes` can never be in a broken state. My histogram only calls `setDataRange()` and `dataToPixel()`. It never touches the scales directly.
- **`LinearScale` (Member 5)** keeps its four range values private, and `setRange()` refuses a zero-width range, which would otherwise cause a division by zero.

**In my own code:**

- The header `histogram.hpp` shows only what other members need: `histogram()`, `histogram_counts()`, `HistogramStyle` and `Orientation`. The helpers `check_edges()` and `fill_box()` are in an unnamed namespace in `histogram.cpp`, so no other file can call them or depend on them. The same is true of `data_ranges()`, `fraction()` and `draw_dot()` in `scatter.cpp`. This is encapsulation at file level instead of class level.
- `HistogramStyle` and `ScatterStyle` are structs with public members on purpose. Any colour, radius or orientation is valid, so there is nothing to protect, and making them private would only add getters and setters.
- `Orientation` is an `enum class`, so only `Vertical` or `Horizontal` can be passed. A plain `int` or `bool` would allow meaningless values.

### Inheritance

**My module does not use inheritance, and neither does the rest of vizlib so far.** No class derives from another. `Axes` *has* two `LinearScale` objects. That is composition ("has a"), not inheritance ("is a"). I made `histogram()` and `scatter()` ordinary functions because they draw once and do not need to remember anything between calls.

Inheritance would fit when the joint plot is built in Week 4. Scatter, histogram, ECDF and KDE are all "a kind of plot that draws onto an `Axes`". A possible design, **not yet implemented**:

```cpp
class Plot {                                   // base class
public:
    virtual ~Plot() = default;
    virtual void draw(Canvas& canvas, Axes& axes) const = 0;   // pure virtual
};

class ScatterPlot : public Plot {              // derived class
private:
    std::vector<double> x_, y_;
    ScatterStyle style_;
public:
    ScatterPlot(std::vector<double> x, std::vector<double> y, ScatterStyle style = {});
    void draw(Canvas& canvas, Axes& axes) const override;
};

class HistogramPlot : public Plot {            // derived class
private:
    std::vector<double> data_, edges_;
    HistogramStyle style_;
public:
    HistogramPlot(std::vector<double> data, std::vector<double> edges, HistogramStyle style = {});
    void draw(Canvas& canvas, Axes& axes) const override;
};
```

`ScatterPlot` and `HistogramPlot` would inherit from `Plot`, and keep their data `private` (encapsulation). The existing `scatter()` and `histogram()` functions would do the actual drawing inside `draw()`. Members 3 (ECDF) and 2 (KDE) could add `EcdfPlot` and `KdePlot` the same way. I will propose this to the team before changing any shared code.

### Polymorphism

**My module does not use polymorphism yet either.** The `Orientation` setting changes how `histogram()` draws, but that is a simple `if` inside one function, not polymorphism.

With the design above, the joint plot could keep its three panels in one list and draw them all the same way:

```cpp
std::vector<std::unique_ptr<Plot>> panels;
panels.push_back(std::make_unique<ScatterPlot>(x, y));
panels.push_back(std::make_unique<HistogramPlot>(x, edges));
for (const auto& p : panels)
    p->draw(canvas, axes);   // calls ScatterPlot::draw or HistogramPlot::draw at run time
```

This is run-time polymorphism: one call, `p->draw()`, runs different code depending on the real type of the object. The joint plot would not need to know which kinds of plot it holds, so a margin could be switched from a histogram to a KDE without changing the joint plot code.

(One small example of compile-time polymorphism already exists in Member 5's code: `Figure::addAxes()` is overloaded, with one version taking no arguments and one taking a `Rect`.)

## 6. Problems I faced and how I solved them

- **The Freedman–Diaconis bin edges are not usable yet.** `histogram_bin_edges()` exists in `src/stats.cpp`, but `stats.cpp` is not in the build, and the class in `stats.hpp` is not the one `stats.cpp` defines. When I tested it, it gave wrong edges for unsorted data, left out the largest value, and returned no edges when the IQR is 0 (for example `{5, 5, 5, 5}`). I listed these failing inputs in PR #57 for Member 2, and made `histogram()` take the edges as a parameter so I was not blocked.
- **`fill_rect` and `fill_circle` are not implemented.** Member 6's `canvas.hpp` declares them, but `canvas.cpp` only defines `set_pixel()` and `get_pixel()`. I wrote a private `fill_box()` helper, the same approach as `draw_dot()` in the scatter plot. Both will be replaced once Member 6's code is merged.
- **No CSV reader or DataFrame on `main`.** Member 1's Week 2 report describes `dataframe.hpp` and `csv_reader.hpp`, but they are not on any branch on GitHub, and there is no `iris.csv`. This is why the scatter plot on real data is not done.
- **Member 5's `Axes` was not in the build.** I added `scale.cpp`, `axes.cpp` and `figure.cpp` to `CMakeLists.txt` in PR #57.

## 7. What is still pending

- Scatter plot on `iris.csv`, once Member 1's CSV reader is merged.
- Switch `histogram()` to Member 2's Freedman–Diaconis edges once they are fixed.
- Switch to `Canvas::fill_rect` and `fill_circle` once Member 6's code is merged.
- Move `scatter()` from a `Rect` to an `Axes`, like the histogram, so the joint plot panels can share scales.
- Propose the `Plot` base class (section 5) to the team for the Week 4 joint plot.
- Nothing in vizlib draws axis lines, ticks or labels yet. This needs an owner (probably Member 5 with Member 8's text renderer).

## AI Use

- **Tool:** Claude (Claude Code)
- **Purpose:** Checked the state of the other members' code, drafted the histogram code, tests and example, ran the tests, and drafted this report.
- **Reason:** To find out quickly which of my dependencies were missing, and to get a working, tested histogram despite them.
