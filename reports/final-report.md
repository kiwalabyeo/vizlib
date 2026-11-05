# vizlib: Final Report

**Group:** 7
**Period:** 28 September to 5 November 2026
**Compiled by:** Member 4 (Kiwalabye Oscar Muwanguzi)
**Repository:** https://github.com/kiwalabyeo/vizlib

## 1. Problem

Plotting data in C++ usually means pulling in a large library, or writing the file reading, statistics, scaling and drawing yourself. We wanted a small library that takes a CSV file and produces a readable statistical plot as a PNG, with nothing to install except a compiler and CMake.

## 2. Objectives

- Read CSV files into a `DataFrame`, and handle messy files: missing cells, quoted commas and bad rows.
- Clean the data: missing values, type conversion and filtering.
- Summarise it: mean, median, spread, quantiles, histogram bins and KDE.
- Draw distribution plots: ECDF, rug, scatter, histogram and joint plots.
- Map data to pixels correctly on linear and log axes, with readable ticks.
- Make the plots readable with titles, axis labels, tick labels, legends and annotations.
- Save PNG images that open anywhere.
- Build on Linux, Windows and macOS, and reject bad input with clear exceptions instead of crashing.

## 3. Library design

The public headers are in `include/vizlib/` and the implementation is in `src/`. The library is built as one static library, `vizlib`, which the tests and examples link against. The modules follow the data from the file to the image:

1. **Data loading** (`csv_reader`, `dataframe`, `preprocessing`; Member 1). CSV text becomes `Column`s in a `DataFrame`.
2. **Statistics** (`stats`, `kde`; Member 2). Summaries, histogram bins and KDE curves.
3. **Plots** (`ecdf` from Member 3; `scatter`, `histogram`, `kde_plot` and `joint_plot` from Member 4). Each plot says what data range it needs and how to draw itself.
4. **Axes and layout** (`scale`, `ticks`, `axes`, `figure`; Member 5). Data values become pixel positions, with ticks and plot areas.
5. **Rendering** (`canvas`, `color`, `geometry`; Member 6). An RGBA image in memory, with lines, rectangles and circles.
6. **Text** (`text`, `annotations`, `legend`, `drawable`; Member 8). Titles, labels, tick numbers, legends and notes, drawn with stb_truetype.
7. **Output, build and integration** (`image_export`, `canvas_export`, CMake, CI, `pipeline.cpp`; Member 7). PNG and PPM files, the build, and the full example.

The full example is `examples/pipeline.cpp`. It reads `data/input/iris.csv`, draws a scatter plot and a histogram with axes, labels, a legend and an annotation, and saves `data/output/pipeline.png`.

The class diagrams are in `docs/uml/`.

## 4. Main functionality

- **Data:**
  - `read_csv()` with `CsvOptions` (delimiter, header, bad-row policy, NA values).
  - Cleaning: `drop_missing`, `fill_missing`, `fill_missing_mean`, `fill_missing_median`, `to_numeric`, `filter_rows`, `select_columns` and others.
- **Statistics:**
  - `summaryStatistics`: mean, median, mode, range, standard deviation, quantiles, IQR and count.
  - Binning through `FreedmanDiaconisBinning` and `SturgesBinning`.
  - `kde()`: a Gaussian kernel with Silverman's bandwidth.
- **Plots:** `ScatterPlot`, `HistogramPlot` (vertical or horizontal), `KdePlot`, `ECDFPlot`, `RugPlot`, and `JointPlot` with histogram or KDE margins.
- **Axes:** linear and log scales, nice ticks, powers of 10 on log axes, axis lines and tick marks, and several axes in one `Figure`.
- **Text:** title, axis labels, tick labels, legend and free-text annotations.
- **Output:** `save_png()` and `save_ppm()` for a whole `Canvas`.

The nine example programs are in `examples/`: `pipeline`, `scatter_example`, `histogram_example`, `joint_plot_example`, `ecdf_example`, `data_examples`, `stats_example` (summaries, both binning rules, a KDE curve on its own), `log_axis_example` (a linear and a log y axis side by side) and `shapes_example` (every Canvas drawing function, saved as PNG and PPM).

## 5. Important implementation decisions (OOP concepts)

- **Encapsulation.**
  - Data members are private, and constructors check their input. For example, `Canvas` checks its size, `ScatterPlot` checks that x and y have the same length, and `DataFrame` refuses duplicate column names.
  - So an object that exists is always valid. `draw()` never has to check again.
  - `TextRenderer` hides stb_truetype behind a private `Impl` pointer, so no other file depends on it.
- **Inheritance and polymorphism (data plots).**
  - `Plot` is an abstract base class, with pure virtual `bounds()` and `draw()` and a virtual destructor.
  - `ScatterPlot`, `HistogramPlot`, `KdePlot`, `ECDFPlot` and `RugPlot` derive from it.
  - `JointPlot` owns three `std::unique_ptr<Plot>` (composition) and calls `draw()` on each one without knowing its type. That is why histogram margins and KDE margins use the same `JointPlot` code.
- **Inheritance and polymorphism (text objects).** `Drawable` is the base class for `Annotations` and `Legend`.
- **Inheritance and polymorphism (binning).** `binningStrategy` is the base class for `FreedmanDiaconisBinning` and `SturgesBinning`, so the binning rule can be swapped without changing the code that uses it.
- **Templates and concepts (C++20).**
  - `vizlib::Numeric` accepts any integer or floating-point type except `bool` and `char`.
  - `ScatterPlot<TX, TY>` is a class template, because it keeps its data. x and y can even have different types.
  - `HistogramPlot`, `ECDFPlot` and `RugPlot` only keep doubles, so only their constructors are templates.
  - The histogram and scatter functions, `JointPlot::with_histograms` and `with_kde`, and `kde()` are function templates.
  - We used templates only where a class takes numeric data. The rendering and text classes stay concrete, and `docs/design-decisions.md` explains why.
- **Errors.** Bad input throws `std::invalid_argument`, and a file that cannot be opened throws `std::runtime_error`, with a message that says what was wrong.
- **No external libraries.** The only third-party code is single-header: doctest, stb_image_write and stb_truetype. It is in `third_party/` with the DejaVu Sans font, so the project builds from a clean clone.

## 6. Testing approach and results

- **Tests are written with doctest,** with one test file per module in `tests/`. Together they form one program, `vizlib_tests`, which CTest runs.
- **CI runs on every push and pull request.** GitHub Actions builds the project on Linux for every push and pull request to `main`, and on Windows and macOS after every push to `main`. It runs the tests, then runs `pipeline` and checks that its PNG exists and is not empty.
- **What the tests cover:**
  - normal use, edge cases (empty data, NaN, equal values, values on bin edges) and invalid input
  - pixel checks on the drawing
  - the templates, where each test runs once for each of 7 number types and must give exactly the same result as `double`
- **Results:** 296 test cases (about 1.45 million assertions) pass in a Release build, with GCC and Clang. The whole suite and all nine examples also run cleanly under AddressSanitizer and UndefinedBehaviorSanitizer.
- **End-of-project testing with AI:** on 3 November we used Claude Code to test the library with bad input on purpose: missing values, infinity, all-equal values, one far outlier, huge sizes and tiny ranges. This found 6 bugs and 5 smaller problems, which were fixed with a new test for each (`tests/test_edge_cases.cpp`). The worst bugs were:
  - wrong statistics when the data had a missing value
  - a histogram that could ask for 10 million bins because of one outlier
  - no bins at all when most values were equal
  
  The full list is in `docs/bug-report.md`.
- **Clean environment:** on 3 November, a fresh clone built, passed every test and ran all six examples by following the README exactly.

## 7. Challenges and how we addressed them

- **Work arrived late and in big batches.** Most of each week's code was opened after the deadline. On 25 October several pull requests merged together broke `main`. Since then, all open pull requests are built and tested together before merging.
- **Code that was never built.** Several `.cpp` files and test files were on `main`, or arrived in pull requests, without being added to CMake. CI stayed green while real errors were hidden (for example, `kde.hpp` did not compile). We found and registered them all, and the number of tests that actually run rose from 126 to 248.
- **Branches that conflicted with `main`.** Members reused old branches, or opened several branches with the same edits. Member 4 rebuilt those pull requests on the current `main`, keeping each author's commits.
- **Dependencies between modules.** The plots needed the Canvas, the axes and the KDE before those were ready. Temporary helpers were used and then replaced by the real modules.
- **The 3-approval rule.** We set a rule that every pull request needs three approvals. In Weeks 3 to 5 it was not kept: the repo owner merged with an override to meet the deadlines, after building and testing.
- **A known limitation on log axes.** The plot helpers `scatter()` and `histogram()` set the axes range from the data plus 5% padding. On a log axis that padding can go below 0, which a log scale rejects. A plot can still be drawn on a log axis by setting the range and the scale first and then calling the plot's `draw()`, which `docs/presentation.md` shows. Making the padding scale-aware is the next fix.
- **Missing features near the deadline.** The ECDF could not be drawn and the plots had no axis lines. Member 5 added the axis lines herself. Member 3 wrote the ECDF drawing and the rug plot herself on 3 November, on Member 4's laptop, so the commit is under his name (see section 8).

## 8. Individual contributions

The commit history and the pull requests on GitHub are the full record. Each member's changelog is in `docs/changelogs/`, and their weekly reports are in `reports/`.

- **Member 1, Kizito Mark James:**
  - `Column` and `DataFrame`
  - the CSV reader with bad-input handling and `CsvOptions`
  - the cleaning functions, `docs/data.md`, the sample CSV files and `data_examples.cpp`, with 49 test cases (#68, #108)
- **Member 2, Natukunda Melissa:**
  - `summaryStatistics` and Freedman–Diaconis binning (#54, #72)
  - the `binningStrategy` classes (#83)
  - `kde()` with Silverman's bandwidth (#107)
- **Member 3, Sengendo Crystal Zawedde:**
  - ECDF coordinates in `compute_coordinates()` (#53)
  - the ECDF tests and example, combined in #90
  - drawing the ECDF as a `Plot`, and the `RugPlot`, which she wrote herself on Member 4's laptop on 3 November; he committed them, so the commit (#122) is under his name
- **Member 4, Kiwalabye Oscar Muwanguzi:**
  - the scatter plot, histogram, `Plot` base class, `KdePlot` and `JointPlot` (#51, #57, #73, #115)
  - C++20, the `Numeric` concept and the templated plots (#92, #93)
  - the repo, its rules, templates and CI setup in Week 1, and coordination
  - integrating and fixing the team's pull requests, and the Week 1 and Week 3 to 5 group reports
  - end-of-project testing and bug fixing with Claude Code (`docs/bug-report.md`)
- **Member 5, Mugabe Destiny:**
  - `LinearScale`, `Axes` and `Figure` (Week 2)
  - `generateTicks()` (#60)
  - the log scale and `generateLogTicks()` (#106)
  - axis lines and tick marks with `drawAxes()` (#121)
- **Member 6, Nambalirwa Flavia Cate:**
  - `Canvas` with Bresenham lines, rectangles and midpoint circles (#91)
  - input validation, clipping and tests (#102)
  - the shared `Color`, `Point` and `Rect` types
- **Member 7, Namaganda Norah Margret:**
  - PNG and PPM export (#48) and the Canvas export adapter (#61)
  - CMake clean-up, the CI pipeline check (#80) and `pipeline.cpp` (#74)
  - the Week 3 OOP assignment report (#77 to #79), the README and licence (#109), and numeric-type tests for the statistics (#110)
- **Member 8, Kabahamba Joy A Maria:**
  - `TextRenderer` with stb_truetype, `Annotations` and `Legend` (#55, #62 to #64, #70)
  - the `Drawable` base class, the figure title and axis labels (#89)
  - tick labels (#98), the pipeline refactor (#100)
  - label fixes, the UML and the design-decisions document (#112)

## 9. AI tools used

Every member used AI tools as support at some point in the project. At the end of the project, AI (Claude Code) was also used to test the finished library and fix the issues found (`docs/bug-report.md`). The full declaration, by member and by week, is in `reports/ai-use-declaration.md`. The main tools were Claude (including Claude Code), ChatGPT and Gemini. They were used to explain topics, draft code and reports for review, and find and fix build problems. Code written for one member by another, or with AI, is named there.

## 10. Lessons learned

- **Agree the interfaces first.** Agreeing the headers in Week 1 let eight people work in parallel. When the agreement slipped (for example, the KDE result format), integration was slow.
- **A green CI only proves what it builds.** Every new `.cpp` and test file has to go into CMake in the same pull request.
- **Small pull requests from a fresh `main` merge cleanly.** Long-lived branches and repeated edits on several branches caused most of our conflicts.
- **An object that checks itself in its constructor is easy to use.** No other code had to guess whether a plot or a canvas was valid.
- **Templates belong where the type is stored.** `ScatterPlot` keeps its data, so it is a class template. `HistogramPlot` only keeps counts, so only its constructor is. A template everywhere would have made the code harder to read for no gain.
- **Review rules need time in the schedule.** Three approvals per pull request did not fit our late deliveries, so the rule was skipped when it mattered most.
