# vizlib: Presentation Guide

Each member presents **their own module only**, on its own, then answers questions and live-codes. Nobody continues someone else's part. Find your section below.

Open this file in VS Code with **Markdown: Open Preview** (`Ctrl+Shift+V`). Every link opens the real file at the right line.

**Where and when:** Saturday 7 November, Marconi Lab.

---

## Before you start (every presenter, on your own laptop)

Run these from the `vizlib` folder, so the build is fresh and nothing compiles for the first time in front of the lecturer:

```
git pull
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure        (Windows: add  -C Debug)
```

Then:

- Run your own example from your section, and have its PNG open.
- Open this guide in preview on one side, and your code on the other.
- Close every other tab and notification. Have no AI tool open.

**Live coding:** the lecturer will ask you to change your own code on the spot: add a small function, make something throw, write a test, change a setting. Type it yourself, rebuild with `cmake --build build`, run it, and explain while you type. The prep doc for your module has practice drills.

---

## Member 1, James: data loading

- **Show:** [`Column`](../include/vizlib/dataframe.hpp#L24), [`DataFrame`](../include/vizlib/dataframe.hpp#L77), [`CsvOptions`](../include/vizlib/csv_reader.hpp#L18), and the [CSV state machine](../src/csv_reader.cpp#L45), which handles commas and line breaks inside quotes.
- **Run:** `./build/examples/data_examples`, which loads and cleans `messy.csv`.
- **OOP to point at:** encapsulation. A `DataFrame` refuses duplicate names and columns of different lengths. `values()` throws on a text column.
- **A test to show:** [uneven rows throw, with the line number](../tests/test_csv_reader.cpp#L83).
- **Docs:** [`docs/data.md`](data.md).

## Member 2, Melissa: statistics and KDE

- **Show:** [`summaryStatistics`](../include/vizlib/stats.hpp#L9), [`binningStrategy`](../include/vizlib/stats.hpp#L38), [`kde()`](../include/vizlib/kde.hpp#L29), with [the Gaussian density at one point](../src/kde.cpp#L8) and [Silverman's bandwidth](../src/kde.cpp#L30).
- **Run:** `./build/examples/joint_plot_example`. The KDE margins on the right are your curves.
- **OOP to point at:** the strategy pattern. `binningStrategy` is abstract, with `FreedmanDiaconisBinning` and `SturgesBinning` deriving from it.
- **A test to show:** [the KDE's area is 1](../tests/test_kde.cpp#L22).

## Member 3, Crystal: ECDF and rug plots

- **Show:** [`ECDFPlot`](../include/vizlib/ecdf.hpp#L32), `compute_coordinates`, and [drawing the steps](../src/ecdf.cpp#L84).
- **Run:** `./build/examples/ecdf_example`. It prints the coordinates, then saves `data/output/ecdf_example.png`.
- **OOP to point at:** `ECDFPlot` inherits from `Plot`, so it can be drawn into any `Axes`.
- **A test to show:** [the steps, checked pixel by pixel](../tests/test_ecdf.cpp#L109).

## Member 4, Oscar: scatter, histogram and joint plots

- **Show:** [`Plot`](../include/vizlib/plot.hpp#L24), [`ScatterPlot<TX, TY>`](../include/vizlib/scatter.hpp#L52), [`HistogramPlot`](../include/vizlib/histogram.hpp#L116), [`JointPlot`](../include/vizlib/joint_plot.hpp#L54), [`with_kde`](../include/vizlib/joint_plot.hpp#L106), and the [`Numeric` concept](../include/vizlib/concepts.hpp#L16).
- **Run:** `./build/examples/joint_plot_example`, with histogram margins and KDE margins.
- **OOP to point at:**
  - [`JointPlot::draw`](../src/joint_plot.cpp#L83) calls `draw()` on three `Plot` pointers. That is runtime polymorphism.
  - Templates: `ScatterPlot` is a class template, while `HistogramPlot` only has a template constructor.
- **A test to show:** [`FakePlot`](../tests/test_joint_plot.cpp#L33), and [the same test run for every number type](../tests/test_plot_templates.cpp#L62).
- **UML:** [`docs/uml/member4-plots.md`](uml/member4-plots.md).

## Member 5, Destiny: axes and scaling

- **Show:** [`Axes`](../include/vizlib/axes.hpp#L15), [`LogScale`](../include/vizlib/scale.hpp#L25), [nice ticks](../src/ticks.cpp#L9), [log ticks](../src/ticks.cpp#L53), and [axis lines and tick marks](../src/axes.cpp#L242).
- **Run:** `./build/examples/pipeline` and point at the axes. Then the log-axis program below.
- **OOP to point at:** encapsulation in the scales, `addAxes()` and `addAxes(Rect)` (overloading), and `std::deque` in `Figure`.
- **A test to show:** `tests/test_log_scale.cpp`.
- **Log-axis demo (tested).** Do the steps in this order:
  ```cpp
  std::vector<double> x{1, 3, 10, 30, 100, 300, 1000}, y{1, 2, 3, 4, 5, 6, 7};
  vizlib::Axes& axes = figure.addAxes();
  axes.setDataRange(1, 1000, 0, 8);                 // 1. range first: a log axis needs it above 0
  axes.setXScale(vizlib::ScaleType::Logarithmic);   // 2. then switch x to log
  vizlib::ScatterPlot plot(x, y);
  plot.draw(canvas, axes);                          // 3. draw into these axes
  figure.drawAxes(canvas);                          //    ticks at 1, 10, 100, 1000
  ```
  Two traps:
  - Calling `setXScale` before `setDataRange` throws.
  - `scatter(canvas, axes, x, y)` pads the range below 0 on a log axis, so draw the plot directly as above.

## Member 6, Flavia: Canvas rendering

- **Show:** [`Canvas`](../include/vizlib/canvas.hpp#L8), [Bresenham line](../src/canvas.cpp#L72), [filled circle](../src/canvas.cpp#L171), and the shared `Color`, `Point` and `Rect` types.
- **Run:** any example. Every dot, bar and line on the picture is drawn by your functions.
- **OOP to point at:** encapsulation. The pixels are private, `set_pixel` clips, `get_pixel` throws, and every coordinate is checked.
- **A test to show:** one from `tests/test_canvas.cpp` that checks individual pixels.

## Member 7, Norah: image export, build, CI and pipeline

- **Show:** [`save_png` for a Canvas](../src/canvas_export.cpp#L41), `image_export.cpp`, [`CMakeLists.txt`](../CMakeLists.txt), the [CI workflow](../.github/workflows/ci.yml#L9), and [`pipeline.cpp`](../examples/pipeline.cpp#L62).
- **Run:** `./build/examples/pipeline`, then `ctest --test-dir build --output-on-failure`.
- **OOP to point at:**
  - two `save_png` overloads: raw bytes, and a `Canvas`
  - the private `to_bytes` helper
  - `const Canvas&`
- **A test to show:** `tests/test_canvas_export.cpp` (the PNG signature and the PPM size).

## Member 8, Joy: text, labels, legends and annotations

- **Show:** [`TextRenderer`](../include/vizlib/text.hpp#L19), its [private `Impl`](../include/vizlib/text.hpp#L45), [drawing letters](../src/text.cpp#L122), [tick labels](../src/axes.cpp#L176), [`Drawable`](../include/vizlib/drawable.hpp#L10), [`Legend`](../include/vizlib/legend.hpp#L21) and [`Annotations`](../include/vizlib/annotations.hpp#L23).
- **Run:** `./build/examples/pipeline`. Point at the title, the labels, the tick numbers, the legend and the note.
- **OOP to point at:**
  - `Drawable` is abstract, with `Legend` and `Annotations` deriving from it.
  - The pimpl idiom in `TextRenderer`.
  - [Why these classes are not templates](design-decisions.md).
- **A test to show:** `tests/test_drawable.cpp`.

---

## Group documents (if the lecturer asks)

- [Final report](../reports/final-report.md)
- [AI-use declaration](../reports/ai-use-declaration.md)
- [Weekly reports](../reports) and [changelogs](changelogs)
