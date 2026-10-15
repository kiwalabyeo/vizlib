# Week 02 Report: Member 4, Scatter Plot

**Name:** Kiwalabye Oscar Muwanguzi 25/U/08733/PS
**Task area:** Joint plots (this week: the scatter plot)
**Branch / pull request:** `feature/scatter-plot`, PR #51

---

## 1. What I did this week

My Week 2 goal was to get a scatter plot drawing onto the Canvas, using hand-picked points instead of real CSV data. I did the following:

- Wrote `scatter()`, which draws one filled dot per (x, y) pair inside a chosen area of the Canvas.
- Wrote tests in `tests/test_scatter.cpp`, following the testing plan in my research file.
- Wrote `examples/scatter_example.cpp`, which draws 10 hand-picked points and saves the result as a PNG using Norah's `save_png()`.
- Added `src/canvas.cpp` and `src/scatter.cpp` to the library in `CMakeLists.txt`, and added an `examples/` build.

## 2. Files I created or changed

| File                            | Purpose                                                    |
| ------------------------------- | ---------------------------------------------------------- |
| `include/vizlib/scatter.hpp`    | Declares `ScatterStyle` and `scatter()`.                   |
| `src/scatter.cpp`               | Maps data to pixels and draws the dots.                    |
| `tests/test_scatter.cpp`        | Tests for the scatter plot.                                |
| `examples/scatter_example.cpp`  | Draws hand-picked points and saves a PNG.                  |
| `examples/CMakeLists.txt`       | Builds the example.                                        |
| `CMakeLists.txt`                | Adds `canvas.cpp`, `scatter.cpp` and the examples option.  |
| `tests/CMakeLists.txt`          | Adds `test_scatter.cpp`.                                   |

## 3. How the scatter plot works

```cpp
void scatter(Canvas& canvas, Rect area,
             const std::vector<double>& x, const std::vector<double>& y,
             const ScatterStyle& style = {});
```

1. **Check the input.** If x and y have different lengths, or are empty, it throws `std::invalid_argument`.
2. **Find the data range.** It finds the min and max of x and of y, ignoring points with a NaN. It then adds 5% padding on each side so no dot sits on the border. If all values are equal, the range is widened by 0.5 each way so there is no division by zero.
3. **Map each point to a pixel.** A value becomes a fraction from 0 (range min) to 1 (range max), then a position inside `area`. Pixel rows count downwards, so the y fraction is flipped to put large y values at the top.
4. **Draw a dot.** Every pixel within `radius` of the centre is coloured using `set_pixel`. Pixels off the canvas are ignored.

The `Rect area` argument stands in for an `Axes` until Member 5's `Axes` class exists. Then `scatter()` will take an `Axes&`, as planned in my research file.

## 4. How I tested it

All 9 test cases (30 assertions) pass, including the existing version and image export tests. My tests check that:

- A known point lands on the expected pixel: with x and y from 0 to 10 on a 100 × 100 area, the point (5, 5) colours pixel (50, 50), and the dot is exactly `radius` pixels wide.
- Large y values are drawn at the top and small ones at the bottom.
- Dots stay inside the given area.
- Different lengths, empty data, and data where every point has a NaN all throw `std::invalid_argument`.
- Points with a NaN are skipped: the plot with a NaN point looks exactly like the plot without it.

The example output, `data/output/scatter_example.png`, shows the 10 points rising from bottom left to top right.

## 5. Problems I faced and how I solved them

- **`fill_circle` is not implemented yet.** Member 6's `canvas.hpp` declares it, but `canvas.cpp` does not define it. I wrote a small private `draw_dot()` helper in `scatter.cpp` using `set_pixel`, marked to be replaced by `Canvas::fill_circle` once it is merged.
- **`canvas.cpp` was not in the build.** I added it to `CMakeLists.txt`. Member 6's pull request will likely make the same change, so one of us will need to resolve a small merge conflict.
- **No `Axes` class yet.** I used a `Rect` for the plot area so I could test the drawing now.
- **No Canvas export adapter on `main` yet.** The example copies the Canvas pixels into an RGBA byte array itself before calling `save_png()`. This will be replaced by Member 7's adapter.

## 6. Class structure

### Did I create any class?

No. `scatter()` is an ordinary function, because it only draws onto a Canvas and does not need to remember anything between calls. Helper functions (`data_ranges`, `fraction`, `draw_dot`) are in an unnamed namespace in `scatter.cpp`, so other files cannot use or depend on them.

### `ScatterStyle` (a struct)

Holds how the dots look: `color` (default blue, 31, 119, 180, the same as seaborn) and `radius` (default 4 pixels). It is a struct because any colour or radius is valid, so there is nothing to protect. Putting the options in a struct keeps the function signature short, and new options (for example a marker shape) can be added later without breaking existing code.

### Types from other members

- `Canvas` (Member 6): I use `set_pixel()` to draw, and the tests use `get_pixel()` to check results.
- `Color`, `Point`, `Rect` (Member 6): the dot colour, the pixel position, and the plot area.

## 7. What is still pending

- Pull request review (needs three approvals).
- Switch to `Canvas::fill_circle` once Member 6's `canvas.cpp` is merged.
- Switch from `Rect` to `Axes` once Member 5's `Axes` class is ready.
- Week 3: histogram in both orientations, and the scatter plot on real CSV data from Member 1's reader.

## AI Use

- **Tool:** Claude (Claude Code)
- **Purpose:** Drafted the scatter plot code, tests, example and this report.
- **Reason:** To get a working, tested scatter plot quickly while the Canvas, Axes and export modules were still unfinished.
