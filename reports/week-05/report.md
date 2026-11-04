# Week 5 Progress Report

**Week:** 26 October to 1 November 2026 (code freeze Saturday 31 October)
**Who compiled the week's report:** Member 4 (Kiwalabye Oscar Muwanguzi)

This was the final week. It also covered the lecturer's templates task, given on 29 October. A few pull requests were merged on 1 and 2 November, just after the freeze. Each item gives its pull request.

## Completed
- **C++20 and the shared concept (Member 4 Oscar, #92):** switched the project to C++20, and added `vizlib::Numeric` (any integer or floating-point type except `bool` and `char`) and `to_doubles()` in `concepts.hpp`, for every templated module to use. Added `VIZLIB_NUMERIC_TYPES` so one test can run once per type with doctest's `TEST_CASE_TEMPLATE`.
- **Templated plots (Member 4 Oscar, #93):**
  - `ScatterPlot<TX, TY>` became a class template.
  - `HistogramPlot` got a template constructor.
  - `scatter`, `histogram`, `histogram_counts`, `auto_bin_edges` and `JointPlot::with_histograms` became function templates.
  - 52 new test cases check that every numeric type draws exactly the same pixels as `double`.
- **Joint plot with KDE margins (Member 4 Oscar, #115, 2 November):**
  - `JointPlot::with_kde(x, y)`, using Member 2's `kde()`.
  - The scatter, histogram and KDE plots now draw with Member 6's Canvas functions instead of temporary helpers.
- **KDE (Member 2 Melissa, opened as #104, merged as #107):** `kde()`, with a Gaussian kernel and Silverman's bandwidth, works on any numeric type. Its grid runs 3 bandwidths past the data so the curve's tails are not cut off.
- **Log scale (Member 5 Destiny, opened as #103, merged as #106):**
  - `LogScale`, a `ScaleType` choice for each axis, and `generateLogTicks()` (1, 10, 100, ...).
  - Zero and negative values are rejected on a log axis.
- **Data loading, complete (Member 1 James, opened as #105, merged as #108):**
  - `Column` and `DataFrame`, with column lookup by name.
  - A rewritten `read_csv()`: quoted cells with commas and line breaks, Windows line endings, a byte order mark, and NA values.
  - `CsvOptions`, and cleaning functions (`drop_missing`, `fill_missing`, `fill_missing_mean`, `to_numeric`, `filter_rows` and others).
  - `docs/data.md`, `messy.csv`, an example, and 49 test cases.
- **Labels and tick labels (Member 8 Joy):**
  - Tick labels on both axes, labels centred on the plot area, `ticks.cpp` added to the build, and the ambiguous `Legend::draw` call fixed (#94, #98).
  - Text documentation (#99), and a pipeline refactor that shows labels, tick labels, a legend and an annotation (#100).
  - On 2 November she kept the axis labels inside the image, added a UML class diagram and `docs/design-decisions.md`, wrote her Week 4 and 5 reports, and cleaned up her changelog (#112, #113, #114).
- **README, licence and tests (Member 7 Norah):** completed README and the MIT licence (#109), numeric-type tests for the statistics (#110), and her changelog for Weeks 4 and 5 (#111).
- **Canvas comments (Member 6 Flavia, #102):** comments on the public Canvas functions in `canvas.hpp`.
- **Test count:** 126 test cases on 30 October, 227 on 2 November. CI passes on Linux, Windows and macOS.

## In Progress
- Axis lines and tick marks. The plots show tick numbers but no axes (Member 5)
- ECDF drawing. `ECDFPlot` works out the coordinates but nothing draws them yet, and the rug plot has not been started (Member 3)
- Week 5 contribution reports from Members 2, 3, 5, 6 and 7 (Members 2, 3, 5, 6, 7)
- UML images for the templates task. Members 4 and 8 have theirs in `docs/uml/` (Members 1, 2, 3, 5, 6, 7)
- Changelog files for Members 1, 5 and 6 in `docs/changelogs/` (Members 1, 5, 6)
- Member 1's Week 3, 4 and 5 reports contained placeholders, such as "[Confirm.]". On 4 November Member 4 replaced each one with the facts from GitHub (pull requests, dates and reviews), or removed it where it could not be checked.

## Challenges/Blockers
- **Code that was never built or tested.**
  - `kde.cpp`, `preprocessing.cpp` and several test files were on `main`, or arrived in pull requests, without being added to CMake. So CI stayed green while real errors were hidden.
  - `kde.hpp` did not compile, and two examples failed to link without `preprocessing.cpp`.
  - Member 5's tests were separate programs using `assert()`, which does nothing in the Release build that CI uses.
  - Member 4 added the missing files, registered the tests, converted Member 5's tests to doctest, and fixed `kde.hpp` (#106, #107, #108). Much of the jump in the test count comes from tests that existed but had never run.
- **Pull requests that conflicted with `main`.**
  - #101, #103, #104 and #105 conflicted with `main`. Member 4 rebuilt each one on the current `main`, keeping the author's commits under their name, and closed the original with an explanation.
  - Member 8 made the same edits on several branches. #95, #96 and #97 were already contained in #98 and were closed.
- **Templates.** The task was clarified as using templates only where they are needed. The group used them where a class takes numeric data: the plots, the KDE and the numeric-type tests. Classes that handle text, pixels or files (text rendering, legend, annotations, Canvas, image export) were left as they are, and their owners explain why in their reports.
- **The 3-approval rule was not kept.** To get the final work onto `main` before the code freeze, the repo owner merged with the override. Before each merge, all open pull requests were built and tested together, and CI was checked after it (Member 4).

## Next Week
Final submission:
- Axis lines and tick marks drawn on every plot (Member 5)
- ECDF drawn as a `Plot`, with an example, and the rug plot if time allows (Member 3)
- Every member: Week 5 contribution report, a UML image of their classes, and a short note on where they used templates or why not (All)
- Missing changelog files added, and Member 1's report placeholders removed (Members 1, 5, 6)
- A final check that the README matches the finished library, and that every example runs from a clean build (Member 7)

## AI Use
- Tool: Claude
- Purpose: Member 4 used it (Claude Code) to design and draft the templates, the concept, the KDE joint plot and their tests, to rebuild and fix the conflicting pull requests, to run the builds, tests, sanitizers and CI checks, and to draw his UML and draft this report and the Week 3 and 4 group reports
- Reason: To integrate and test the whole team's work before the code freeze while finishing his own module
- Used by: Member 4 (Oscar)

- Tool: Gemini
- Purpose: As declared: `std::optional` interface design and the report structure. This is the same text as his Week 3 and 4 reports
- Reason: To speed up header design and the report
- Used by: Member 1 (James)

- No AI section in the contribution report: Member 8 (Joy). Added on 4 November, saying no AI use was recorded.
- No Week 5 contribution report yet: Members 2, 3, 5, 6 and 7
