# vizlib: Presentation Guide

Open this file in VS Code with **Markdown: Open Preview** (`Ctrl+Shift+V`). Every link opens the real file at the right line, so the whole presentation runs from here, in the order the lecturer asked for.

**Slot:** Saturday, Marconi Lab, about 30 minutes. Each part has a rough time.

---

## Before you start (5 minutes before the slot)

Run these once from the `vizlib` folder, so the build is fresh and nothing is compiled live by surprise:

```
git pull
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure        (Windows: add  -C Debug)
./build/examples/pipeline                          (Windows: .\build\examples\pipeline.exe)
./build/examples/joint_plot_example
./build/examples/ecdf_example
```

Then:
- Open this guide in preview on one side, and the code on the other.
- Close every other tab and notification.
- Have the three PNGs ready in `data/output/`.

---

## 1. Problem (1 min, Member 4)

Plotting data in C++ normally means a large library, or writing the file reading, statistics, scaling and drawing yourself. vizlib turns a CSV file into a readable statistical plot saved as a PNG, with only a compiler and CMake.

- [Final report, section 1](../reports/final-report.md#1-problem)

## 2. Objectives (1 min, Member 4)

- [README objectives](../README.md#objectives)
- **The point to say:** load, clean, summarise, plot, scale, label and save, on Linux, Windows and macOS, with clear errors for bad input.

## 3. Library design (3 min, Member 4, then Member 7 for the build)

- **Layout:**
  - public headers in [`include/vizlib/`](../include/vizlib)
  - code in [`src/`](../src)
  - one test file per module in [`tests/`](../tests)
  - programs that use the library in [`examples/`](../examples)
- **The modules follow the data:** CSV → `DataFrame` → statistics → plots → axes → `Canvas` → text → PNG. See [Final report, section 3](../reports/final-report.md#3-library-design).
- **Class diagrams:**
  - [plots (Member 4)](uml/member4-plots.md)
  - [figure, axes, text (Member 8)](uml/week-04-class-diagram.md)
- **Build (Member 7):**
  - [`CMakeLists.txt`](../CMakeLists.txt): one static library, `vizlib`, with C++20.
  - [CI workflow](../.github/workflows/ci.yml#L9): build and test on Linux for every pull request and push to `main`, then run the pipeline and check that the PNG exists. Windows and macOS are checked after every push to `main`.

## 4. Main functionality (12 min, every member, about 90 seconds each, in data order)

Each member shows their header, one key function, and what it produces.

1. **Data loading, Member 1 (James)**
   - [`DataFrame`](../include/vizlib/dataframe.hpp#L77), [`Column`](../include/vizlib/dataframe.hpp#L24)
   - [`CsvOptions`](../include/vizlib/csv_reader.hpp#L18), and the [CSV reader's state machine](../src/csv_reader.cpp#L45), which handles commas and line breaks inside quotes
   - Show: `./build/examples/data_examples` on `messy.csv`
2. **Statistics and KDE, Member 2 (Melissa)**
   - [`summaryStatistics`](../include/vizlib/stats.hpp#L9), [`binningStrategy`](../include/vizlib/stats.hpp#L38)
   - [`kde()`](../include/vizlib/kde.hpp#L29), with [the Gaussian density at one point](../src/kde.cpp#L8) and [Silverman's bandwidth](../src/kde.cpp#L30)
3. **Axes and scaling, Member 5 (Destiny)**
   - [`Axes`](../include/vizlib/axes.hpp#L15), [`LogScale`](../include/vizlib/scale.hpp#L25)
   - [nice ticks](../src/ticks.cpp#L9), [log ticks](../src/ticks.cpp#L53)
   - [axis lines and tick marks](../src/axes.cpp#L242)
4. **Rendering, Member 6 (Flavia)**
   - [`Canvas`](../include/vizlib/canvas.hpp#L8)
   - [Bresenham line](../src/canvas.cpp#L72), [filled circle](../src/canvas.cpp#L171)
5. **Text, labels and legends, Member 8 (Joy)**
   - [`TextRenderer`](../include/vizlib/text.hpp#L19), [drawing glyphs](../src/text.cpp#L122)
   - [tick labels](../src/axes.cpp#L176)
   - [`Legend`](../include/vizlib/legend.hpp#L21), [`Annotations`](../include/vizlib/annotations.hpp#L23)
6. **ECDF and rug plots, Member 3 (Crystal)**
   - [`ECDFPlot`](../include/vizlib/ecdf.hpp#L32), [drawing the steps](../src/ecdf.cpp#L84)
   - Show: `data/output/ecdf_example.png`
7. **Scatter, histogram and joint plots, Member 4 (Oscar)**
   - [`ScatterPlot<TX, TY>`](../include/vizlib/scatter.hpp#L52), [`HistogramPlot`](../include/vizlib/histogram.hpp#L115)
   - [`JointPlot`](../include/vizlib/joint_plot.hpp#L54), [`with_kde`](../include/vizlib/joint_plot.hpp#L106)
   - Show: `data/output/joint_plot_example.png`
8. **Export and the full pipeline, Member 7 (Norah)**
   - [`save_png` for a Canvas](../src/canvas_export.cpp#L41)
   - [`pipeline.cpp`](../examples/pipeline.cpp#L62): every module in one program
   - Show: `data/output/pipeline.png`

## 5. Important implementation decisions: OOP concepts (5 min)

Each concept is presented by the member whose code shows it best.

- **Inheritance and runtime polymorphism (Member 4).**
  - [`Plot`](../include/vizlib/plot.hpp#L24) is abstract.
  - [`JointPlot::draw`](../src/joint_plot.cpp#L83) calls `draw()` on three `Plot` pointers without knowing their types.
  - The [`FakePlot` test](../tests/test_joint_plot.cpp#L33) proves it: a plot type defined only inside a test works in a joint plot.
- **Inheritance and polymorphism, second hierarchy (Member 8).** [`Drawable`](../include/vizlib/drawable.hpp#L10) is the base of `Annotations` and `Legend`.
- **The strategy pattern (Member 2).** [`binningStrategy`](../include/vizlib/stats.hpp#L38), with the Freedman–Diaconis and Sturges rules.
- **Encapsulation (Members 6 and 1).**
  - `Canvas` keeps its pixels private and checks every coordinate.
  - `DataFrame` refuses duplicate names and columns of different lengths.
  - `TextRenderer` hides stb_truetype behind a [private `Impl`](../include/vizlib/text.hpp#L45).
- **Templates and concepts (Member 4).**
  - The [`Numeric` concept](../include/vizlib/concepts.hpp#L16).
  - `ScatterPlot<TX, TY>` is a class template because it stores its data. `HistogramPlot` only has a template constructor because it only keeps counts.
  - [Why the rendering classes are not templates (Member 8)](design-decisions.md).

## 6. Demonstration with live coding (4 min, Member 7 drives, the owner of each change types it)

Run the pipeline, open the PNG, then make **one small, safe change live**, rebuild and run again:

- **Scatter style (Member 4).** In `pipeline.cpp`, give the scatter a red `ScatterStyle` with `radius = 6`, rebuild, run, and show the new picture.
- **Log axis (Member 5).** In a new small program, or at the end of `main` in an example, do the steps in this order (tested):
  ```cpp
  std::vector<double> x{1, 3, 10, 30, 100, 300, 1000}, y{1, 2, 3, 4, 5, 6, 7};
  vizlib::Axes& axes = figure.addAxes();
  axes.setDataRange(1, 1000, 0, 8);                 // 1. range first: a log axis needs it above 0
  axes.setXScale(vizlib::ScaleType::Logarithmic);   // 2. then switch x to log
  vizlib::ScatterPlot plot(x, y);
  plot.draw(canvas, axes);                          // 3. draw into these axes
  figure.drawAxes(canvas);                          //    ticks at 1, 10, 100, 1000
  ```
  The points come out evenly spaced. Two traps:
  - Calling `setXScale` before `setDataRange` throws, because the default range 0..1 includes 0.
  - `scatter(canvas, axes, x, y)` sets its own range with 5% padding, which goes below 0 on a log axis, so draw the `ScatterPlot` directly as above.
- **Joint plot (Member 4).** Swap `with_histograms(x, y)` for `with_kde(x, y)` in `joint_plot_example.cpp`. The `JointPlot` code does not change.
- **Legend (Member 8).** Add a second legend entry, or change the annotation text.

Rehearse each change once, so you know exactly which line to edit.

## 7. Testing approach and results (2 min, Member 7)

- **doctest:** one test file per module, together in one program run by CTest. CI runs it on every push.
- **Show one test per idea:**
  - [a pixel check on the ECDF steps](../tests/test_ecdf.cpp#L109)
  - [the same test run once per number type](../tests/test_plot_templates.cpp#L62)
  - [bad CSV input](../tests/test_csv_reader.cpp#L13)
  - [the KDE's area is 1](../tests/test_kde.cpp#L22)
- **Results:** 248 test cases pass. A fresh clone builds and passes by following the README. Run `ctest --test-dir build --output-on-failure` live.

## 8. Challenges and how we addressed them (2 min, Member 4)

- [Final report, section 7](../reports/final-report.md#7-challenges-and-how-we-addressed-them)
- **Say it honestly:**
  - work arrived late
  - files were left out of CMake, so CI looked green while errors were hidden
  - branches conflicted, and were rebuilt keeping each author's commits
  - the 3-approval rule was skipped near the deadlines

## 9. Individual contributions (in section 4, plus 1 min here)

- [Final report, section 8](../reports/final-report.md#8-individual-contributions), [changelogs](changelogs), [weekly reports](../reports)
- The GitHub commit history and pull requests are the evidence.

## 10. AI tools used (1 min, Member 4)

- [AI-use declaration](../reports/ai-use-declaration.md)
- Name the tools, what each member used them for, and that every member can explain the code in their module.

## 11. Lessons learned (1 min, any member)

- [Final report, section 10](../reports/final-report.md#10-lessons-learned)
