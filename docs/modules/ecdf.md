# ECDF and rug plots

- **Owner:** Sengendo Crystal Zawedde (Member 3)
- **Header:** `include/vizlib/ecdf.hpp`
- **Source:** `src/ecdf.cpp`
- **Tests:** `tests/test_ecdf.cpp` (12 test cases; the number-type one runs once for each of 7 types)
- **Example:** `ecdf_example`, which prints the ECDF points of a small list, then draws the ECDF and rug of iris petal length and saves `data/output/ecdf_example.png`

## 1. What it does

- **`ECDFPlot`** draws the empirical cumulative distribution function (ECDF). For each value x, it shows what share of the data is less than or equal to x. The curve goes from 0 to 1 in steps.
- **`RugPlot`** draws one short vertical mark at the bottom of the plot for every value, so you can see where the data actually lies. It is usually drawn under an ECDF or a histogram.

```cpp
#include <vizlib/ecdf.hpp>

const vizlib::ECDFPlot ecdf(petal);   // petal is a std::vector of any number type
const vizlib::RugPlot rug(petal);
axes.setDataRange(ecdf.bounds().x_min, ecdf.bounds().x_max, 0.0, 1.05);
ecdf.draw(canvas, axes);
rug.draw(canvas, axes);
```

## 2. How it works

**Working out the points** (`compute_coordinates`):

1. Copy the data and leave out NaN values.
2. Sort the copy from smallest to largest.
3. The i-th value (counting from 0) gets the point $\left(x_i, \frac{i+1}{n}\right)$, where n is the number of values.

So `{3, 1, 2}` gives (1, 0.333), (2, 0.667) and (3, 1). The last point is always at 1, because all of the data is at or below the largest value.

**Drawing the steps** (`ECDFPlot::draw`). The curve starts at 0. At each value it goes **up** to that value's y. Between two values it goes **across**, flat, at the height it already reached. A value that appears twice makes a flat part of length 0, so the jump there is $\frac{2}{n}$ instead of $\frac{1}{n}$. Every point goes through `axes.dataToPixel`, so the plot works on any axes.

**Bounds.** x runs from the smallest to the largest value, plus 5% padding on each side (`padded_range`). y runs from 0 to 1.05, so the top step does not touch the border.

**The rug** (`RugPlot::draw`) takes the bottom edge of the plot area. For each value it finds the x pixel and draws a line 10 pixels up from the edge. It finds the y value at the bottom edge with `pixelToData`, so it also works on a log y axis, where 0 is not allowed.

## 3. Why it is written this way

- **Inheritance.** Both classes derive from `Plot`, the same base class as `ScatterPlot` and `HistogramPlot`. They override `bounds()` and `draw()`. So an ECDF can be drawn into any `Axes`, or used as a panel of a `JointPlot`, with no change to that code. That is runtime polymorphism.
- **A template constructor.** `template <Numeric T>` lets you pass a `std::vector<int>`, `float` and so on. It converts the data once with `to_doubles()` and calls the `double` constructor. The class itself is not a template, because it only keeps `double` points.
- **Encapsulation.** The points and the style are private. The constructor throws `std::invalid_argument` if the data is empty or only NaN, so an `ECDFPlot` made with data can always be drawn.
- **Style structs.** `EcdfStyle` and `RugStyle` hold the colour, thickness and mark height, with defaults. So `ECDFPlot(data)` works with no extra settings.
- **`const`.** `compute_coordinates`, `bounds()` and `draw()` do not change the plot.
- **NaN is left out first.** `std::sort` needs values that can be compared, and NaN is neither smaller nor bigger than anything.

## 4. How it is tested

- **Normal use:** unsorted data is sorted, and each step goes up by $\frac{1}{n}$. The bounds are the data range plus 5%, and 0 to 1.05.
- **Edge cases:** an empty vector gives no points. NaN values are left out. A default `ECDFPlot` draws nothing.
- **Drawing:** pixels are checked to show a step going up at each value and flat in between. The rug draws one mark per value on the bottom edge, also on a log y axis.
- **Bad input:** empty data, data that is only NaN, a thickness below 1 and a mark height of 0 all throw.
- **Polymorphism and templates:** the ECDF is drawn through a `Plot` pointer, and gives the same steps for all 7 number types.

## 5. Limitations

- No complementary ECDF (1 − ECDF) and no option to show percentages instead of 0 to 1.
- The rug marks are only on the bottom edge, not on the left side for y values.
- With a lot of data, the rug marks overlap into a solid block.

## 6. Who changed it

- **Crystal:**
  - the first header (#33, merged 12 October) and `compute_coordinates` in `ecdf.cpp` (#53, merged 19 October)
  - the first ECDF tests and the example program (#87, #88, 28 October), combined in #90 by Oscar on 29 October, keeping her commits
  - the `ECDFPlot` as a `Plot`, the `RugPlot`, the template constructors, and the helpers `without_nan` and `padded_range`, written with Oscar on his laptop on 3 November. So they reached GitHub in Oscar's commit (#122), which was made with Claude Code.
- **Oscar:** fixed the ECDF build (the `vizlib` namespace, doctest tests, registering the example) on 29 October, and drew the axes in the ECDF example.
- **This guide** was written by Member 4 with Claude Code, from the code and the tests.
