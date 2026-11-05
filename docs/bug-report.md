# Bug Report: End-of-Project Testing

**Date:** 3 November 2026
**Done by:** Member 4 (Kiwalabye Oscar Muwanguzi), using Claude Code (AI)
**Pull request:** "Fix bugs found in edge-case testing"

At the end of the project we tested the whole library for errors, beyond what the normal tests cover. This testing, and the fixes below, were done with an AI tool, Claude Code. This is declared in `reports/ai-use-declaration.md`.

## How we tested

1. **Strict compiler warnings.** We built with GCC 13 and Clang 18, with extra warnings on (`-Wall -Wextra -Wpedantic -Wshadow -Wconversion`).
2. **Sanitizers.** We built with AddressSanitizer and UndefinedBehaviorSanitizer, then ran every test and all six examples. These catch memory errors and undefined behaviour while the program runs.
3. **Bad input on purpose.** A probe program called about 80 public functions with inputs a user could realistically give:
   - missing values (NaN), as a CSV with empty cells produces
   - infinity
   - all-equal values
   - one far outlier
   - huge or zero sizes
   - very small or very large ranges
   - broken CSV text
4. **Static analysis.** clang-tidy, with its bug-finding and performance checks.
5. **Looking at the pictures.** We opened every PNG the examples produce.

## Results before the fixes

Normal use was clean:
- all 248 tests passed on both compilers
- the sanitizers found nothing in the tests or the examples
- clang-tidy found no real bugs

The bad-input testing found the 6 bugs and 5 smaller problems below.

## Bugs fixed

### Bug 1: missing values gave wrong statistics silently

- **Module:** statistics (`stats.hpp`, Member 2)
- **What happened:** `summaryStatistics` kept NaN values. The CSV reader stores a missing cell as NaN, so this happens with any CSV that has an empty cell. `std::sort` cannot order NaN, so the sorted data came out in the wrong order. For example, the median of {5, 4, 3, 2, 1} with one value missing came out as 2, 3 or 4, depending on which value was missing. The mean and standard deviation came out as NaN.
- **Fix:** `collect_data` now leaves out NaN values, like the plots already did. It throws `std::invalid_argument` if no value is left, or if the data contains infinity. `binningStrategy` does the same. The shared helper is `values_without_nan` in `stats.hpp`.

### Bug 2: one far outlier could use gigabytes of memory

- **Module:** statistics (`stats.cpp`, Freedman–Diaconis rule, Member 2)
- **What happened:**
  - Take 1,000 values between 0 and 1, plus one value of 1,000,000. The bin width comes from the middle half of the data, so it is tiny, about 0.1. The range is huge, so the rule asked for **10 million bins**.
  - With an outlier of 1e12 the bin count did not fit in an `int`, which is undefined behaviour. It came out as 0 bins.
- **Fix:** the bin count is now worked out as a `double` first and capped at 1,000. When the cap applies, the bins are made wider so they still cover the whole range.

### Bug 3: data that barely varies gave no bins

- **Module:** statistics (`stats.cpp`, Member 2)
- **What happened:**
  - When more than half the values are equal, the IQR is 0. Freedman–Diaconis then returned a single edge, which means no bins at all.
  - When all values are equal, Sturges returned edges that were all the same number.
  - In both cases, drawing the histogram threw an error. `examples/pipeline.cpp` uses these edges directly.
- **Fix:**
  - When the IQR is 0, Freedman–Diaconis now uses Sturges' rule instead.
  - When all values are equal, both rules return one bin of width 1 around the value.
  - This matches what `auto_bin_edges` already did.

### Bug 4: one NaN made the whole KDE curve NaN

- **Module:** KDE (`kde.cpp`, Member 2)
- **What happened:**
  - Data with a missing value gave a curve that was NaN at every point. No error was raised, and the plot was empty.
  - A bandwidth of NaN or infinity was also accepted, with the same result.
- **Fix:**
  - `kde()` and `silverman_bandwidth()` now leave out NaN values.
  - A bandwidth that is not a finite number now throws `std::invalid_argument`.
  - `gaussian_density_at` now also refuses a NaN bandwidth. The old check, `h <= 0`, is false for NaN, so NaN got through.

### Bug 5: a huge Canvas overflowed an int

- **Module:** rendering (`canvas.cpp`, Member 6)
- **What happened:** the number of pixels was worked out as `width * height` in `int`. For 100,000 × 100,000 that overflows, which is undefined behaviour. The sanitizer reported it.
- **Fix:**
  - The product is now worked out as `std::size_t`.
  - A side longer than 20,000 pixels is refused with `std::invalid_argument`. At that size the image already needs 1.6 GB.

### Bug 6: infinity gave broken histogram edges

- **Module:** plots (`histogram.cpp`, `auto_bin_edges`, Member 4)
- **What happened:** data containing infinity gave the edges NaN, inf, inf, inf, with no error.
- **Fix:** infinity now throws `std::invalid_argument`.

## Smaller problems fixed

1. **Tick labels that all looked the same** (`ticks.cpp` and `axes.cpp`, Members 5 and 8)
   - **Before:** tick labels were cut to 4 significant digits (`setprecision(4)`). So close ticks printed the same text: 12345, 12350 and 12355 all showed as "1.234e+04", and 1e15, 1e15+5 and 1e15+10 all showed as "1e+15".
   - **Now:** the new function `formatTickLabel` picks the number of decimals from the gap between ticks. A gap of 5 needs no decimals, and a gap of 0.5 needs one, so the ECDF axis now reads 0.0, 0.5, 1.0. Log axes keep the old format.
2. **A short log axis had no ticks** (`ticks.cpp`, Member 5)
   - **Before:** a log axis from 2 to 5 contains no power of 10, so it got no ticks at all. From 20 to 50 it got only one.
   - **Now:** these ranges fall back to ordinary nice ticks inside the range.
3. **A very long log axis had hundreds of ticks** (`ticks.cpp`, Member 5)
   - **Before:** an axis from 1e-300 to 1e300 got 601 ticks, all drawn on top of each other.
   - **Now:** powers of 10 are skipped to keep about 10 ticks.
4. **Ticks drifted and huge ranges gave no ticks** (`ticks.cpp`, Member 5)
   - **Drift:** ticks were made by adding the step again and again, so rounding errors built up. clang-tidy flagged the floating-point loop counter. Each tick is now worked out as k × step for a whole number k.
   - **Huge ranges:** a range like -1e308 to 1e308 overflowed to infinity and gave an empty list. It now throws.
5. **A NaN font size was accepted** (`text.cpp`, Member 8)
   - **Before:** the check `fontSize <= 0` is false for NaN, so a NaN or infinite font size got through.
   - **Now:** both are refused.

We also renamed a constructor parameter `Data` to `data` in `stats.hpp`, because it hid the member of the same name (`-Wshadow`).

## Tests added

There is one new file, `tests/test_edge_cases.cpp`, with 15 test cases: one or more per bug and per smaller fix. Each test names the bug it checks. The full suite is now **263 test cases**. They all pass with GCC and Clang, and under AddressSanitizer and UndefinedBehaviorSanitizer.

## Second check (4 November)

The first round left the pictures without axes in three examples. We had listed that as "looks only", which was wrong: a plot with no axes cannot be read. So we checked again, more widely, and fixed what we found:

1. **Three examples drew no axes** (`scatter_example`, `histogram_example`, `joint_plot_example`).
   - **Cause:** the library had `drawAxes` and `drawTickLabels`, but these examples never called them.
   - **Fix:** they now draw axis lines, tick marks, tick numbers, a title and axis labels. The joint plots get axes on their centre panel through `JointPlot::layout()`.
2. **The Windows run command could not work.**
   - **Cause:** with Visual Studio, the usual Windows setup, the programs were built into `build\examples\Debug\`. So `.\build\examples\pipeline.exe`, as the README and the presentation guide say, found nothing. CI did not catch it, because on Windows and macOS it ran the tests but not the pipeline.
   - **Fix:** `examples/CMakeLists.txt` now puts every example straight in `build/examples` on every system. CI now runs the pipeline and checks its PNG on Windows and macOS too.
3. **Every build printed 24 warnings**, all from the third-party file `stb_image_write.h`. They are not our code, but they look bad when building live. The `third_party/` folder is now included as a SYSTEM folder, so the build prints no warnings.
4. **Code that no test ran.** A coverage report showed which lines the tests reach. Overall it was 92%, but some whole functions were never called by a test:
   - `Axes::drawTickLabels` (the tick numbers)
   - `RugPlot::bounds`
   - the size and margin checks in `Figure`
   - `Figure::getAxes` out of range
   - `Legend::clear`
   
   New file: `tests/test_untested_parts.cpp`, with 8 test cases, including one for a custom `JointLayout`, which nothing had used. The suite is now **271 test cases**.
5. **Features that worked but no example showed.** We listed every public function, then checked who calls it: the examples, the tests, other library code, or nothing.
   - **Called by nothing:** only `TextRenderer`'s move assignment. It is there on purpose (the rule of five), so it stays.
   - **Called only by tests:** these features worked, but no example ever showed them running:
     - the log scale (Member 5)
     - the statistics summaries and the two binning classes used directly (Member 2)
     - `KdePlot` on its own and `histogram_counts` (Member 4)
     - `draw_rect` and `draw_circle` (Member 6)
     - `save_ppm` (Member 7)
     - the filtering, column and missing-value functions (Member 1)

   Three new examples show them: `stats_example`, `log_axis_example` and `shapes_example`. `data_examples` now also shows `filter_numeric`, `filter_rows`, `select_columns`, `drop_columns`, `fill_missing`, `fill_missing_mean`, `count_missing` and `parse_csv`.

We also ran every live-coding drill in the prep docs and the presentation guide. Each one compiles and gives the answer the doc says.

## Third check (5 November): was the data module really used?

A team member noticed that Member 1's data module looked like it stood on its own. We checked who calls each of its functions.

- **What we found:**
  - `read_csv` and `DataFrame` were used by six examples, so every plot does start from Member 1's reader.
  - The cleaning functions (`drop_missing` and the rest) were only used in `data_examples`.
  - The examples copied columns by hand with their own helpers (`column_as_doubles` in `pipeline.cpp`, `examples/example_utils.hpp`) instead of using `df["name"]` and `drop_missing`. `column_as_doubles` also skipped missing cells in each column separately, so a row with a missing value in only one column would have paired the wrong x with the wrong y.
  - Four modules had their own copy of the same "leave out NaN" code: the ECDF and rug plot, the joint plot's KDE margins, the statistics, and `preprocessing.cpp` itself.
- **What we changed:**
  - Added `drop_nan(values)` to Member 1's `preprocessing.hpp`: the one place the library leaves missing values out of a list of numbers. The ECDF, the rug plot, the joint plot and the statistics now call it, and the three copies were removed. The statistics still refuse infinity on top of it.
  - `pipeline`, `scatter_example`, `ecdf_example` and `joint_plot_example` now read the file, call `drop_missing`, and take each column with `df["name"]`. The hand-made helpers were deleted.
  - Two new tests for `drop_nan`. The suite is now **273 test cases**. Every example PNG is byte-for-byte the same as before, because the sample files have no missing values.
- **Why the plots still take `std::vector<double>`:** a plot should not need a `DataFrame`. Someone with numbers from anywhere else can still plot them. The data module sits at the front of the pipeline, the same way pandas feeds matplotlib in Python.

## Fourth check (5 November): is everyone's code used?

For each member's headers we checked which other modules and which examples include them.

- **Member 1 (data):** `read_csv` starts six examples, `drop_missing` cleans the data in five, and `drop_nan` is called by the ECDF, joint plot and statistics.
- **Member 2 (statistics):** the histogram gets its bins from `summaryStatistics`, the KDE plot and joint plot use `kde()`, and `pipeline` and `stats_example` use both.
- **Member 3 (ECDF):** `ECDFPlot` and `RugPlot` are drawn in `ecdf_example`. Because they are a `Plot`, a `JointPlot` could also use them as a panel.
- **Member 4 (plots):** every plot derives from `Plot`, and the scatter and histogram are reused inside the joint plot.
- **Member 5 (axes):** every plot draws through `Axes`, `Axes` uses the scales and the tick functions, and three examples use `Figure`.
- **Member 6 (Canvas):** every drawing module draws on the `Canvas`, and every example creates one.
- **Member 7 (export):** every example saves its picture with `save_png`, which calls `image_export`.
- **Member 8 (text):** the axes and the figure draw their numbers and labels with `TextRenderer`, and every example uses it.

One gap: `Drawable` was only used through a pointer in the tests. `pipeline` now draws its note and legend in one loop through `Drawable` pointers, so the polymorphism runs in a real program too. The PNG is byte-for-byte the same.

What is left unused outside the tests is small: getters such as `Figure::getTitle`, options such as `CsvOptions` and `BadRowPolicy`, and the old names `get_column`, `row_count` and `handle_missing_values`, which are kept so code written for the first version still compiles.

## Known limitations not changed

These are design limits, not crashes, so we left them as they are:

- `scatter()` and `histogram()` pad the data range by 5%. On a log axis that padding can go below 0, which a log scale refuses. The workaround is in `docs/presentation.md`: set the range and the log scale first, then call the plot's `draw()`.
- Calling `Axes::setXScale(Logarithmic)` before `setDataRange` throws, because the default range starts at 0.
- `save_png` and `save_ppm` return `false` on failure instead of throwing, unlike the rest of the library.
- About 25 compiler warnings remain about number conversions, such as `int` to `float`, under `-Wconversion`. None of them changes a result for the sizes this library uses.

## Ideas for later

- **Examples:** the margin panels of the joint plots have no tick numbers.
- **Pipeline layout:** in `pipeline.png`, the y-axis label sits in the top-left corner, and the right-hand histogram has no axis labels.
- **CI:** run the sanitizer build, and the Windows and macOS jobs, on every pull request instead of only after merging.
