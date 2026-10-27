# Week 3 Report: Pipeline Integration

**Name:** Namaganda Norah Margret 25/U/0846 **Task area:** Pipeline (connecting the library modules into one example program) **Branch / pull request:** week3/pipeline, PR #61

## 1. What I did this week

My task for Week 3 was to get the build files in order and to create a pipeline that connects the different modules of the library. I did the following:

- Checked the root `CMakeLists.txt` and confirmed that every source file it lists exists. I added `src/stats.cpp` and `ecdf.cpp` to the library and removed lines that were listed twice.
- Checked `tests/CMakeLists.txt` and `examples/CMakeLists.txt`. I removed duplicate test lines and confirmed that every test file in the `tests` folder is in the list, that the tests are registered with CTest, and that `pipeline` is built and linked to `vizlib`.
- Checked the third-party libraries in `third_party` (doctest, stb\_image\_write, stb\_truetype and the DejaVuSans font) and confirmed that they are all present and reachable from the build.
- Created the first version of `examples/pipeline.cpp`. It generates sample data, draws a scatter plot and a histogram side by side, and saves one PNG with the Canvas export adapter.
- Integrated more modules into the pipeline. The histogram bin edges now come from the `stats` module, and each plot has a title drawn with the `text` module.
- Found and fixed a problem in `stats.hpp` and `stats.cpp` that stopped the pipeline from linking (see Section 6).
- Rebuilt everything, ran the pipeline and ran all the tests.
- Added a new entry to my changelog in `docs/changelogs/Namaganda-Norah.md`.
- Committed the work on the `week3/pipeline` branch.

## 2. Files I created or changed

| File | Purpose |
| --- | --- |
| `examples/pipeline.cpp` | The pipeline example. It generates data, gets bin edges from `stats`, draws both plots and their titles, and saves the PNG. |
| `examples/CMakeLists.txt` | Registers `pipeline` as an executable linked to `vizlib`. |
| `include/vizlib/stats.hpp` | Now holds the single `summaryStatistics` class inside `namespace vizlib`, with `#pragma once`. |
| `src/stats.cpp` | Removed the duplicate copy of the class. It now includes `vizlib/stats.hpp` and only contains the function definitions. |
| `CMakeLists.txt` | Added `src/stats.cpp` and `ecdf.cpp` to the library and removed duplicate source lines. |
| `tests/CMakeLists.txt` | Removed duplicate test lines. |
| `ecdf.cpp` | Changed its include to `<vizlib/ecdf.hpp>`. |
| `include/vizlib/ecdf.hpp` | New header that `ecdf.cpp` now includes. It was untracked, so I committed it with the other files. |
| `docs/changelogs/Namaganda-Norah.md` | New entry for 2026-10-26. |

**Not committed:** `reports/week-01.md`. It is not part of this work.

## 3. How the pipeline works

The pipeline in `examples/pipeline.cpp` runs in five steps:

1. **Data.** It creates 60 points. Each `y` value follows a rising line (`0.5 * x`) with random noise added. A fixed seed (42) gives the same picture on every run.
2. **Bin edges.** It puts the `y` values into a `summaryStatistics` object, sorts them with `sorted()`, and calls `histogram_bin_edges()` to get the edges for the histogram.
3. **Canvas.** It creates a 700 x 300 white canvas with two light grey plot areas side by side, so I can see where each plot should stay.
4. **Plots and titles.** It draws the scatter plot in the left area and the histogram in the right area. Then it uses `TextRenderer` with the DejaVuSans font to write "Scatter plot" and "Histogram" above them.
5. **Save.** It calls `save_png()` from the Canvas export adapter to write `data/output/pipeline.png`. It prints the path if saving works and returns 1 with an error message if it fails.

The program must be run from the project root, because the font path and the output path are relative.

## 4. The data and bin format

- The data is a `std::vector<double>` for `x` and one for `y`.
- `histogram_bin_edges()` expects **sorted** data. It uses the bin width `2 * IQR / (number of values)^(1/3)`, starts at the smallest value and returns a vector of edges. This is why the pipeline sorts `y` before calling it.
- The canvas stores colours as `Color` values with `r`, `g`, `b` and `a`, each from 0 to 255, the same format agreed in Week 2.

## 5. How I tested it

- I deleted the build folder and configured the project from scratch with CMake, then built it with MSVC.
- I ran a small PowerShell check that every `.cpp` file listed in the root `CMakeLists.txt` exists. Nothing was reported missing.
- I ran a second check that every `test_*.cpp` file in the `tests` folder is listed in `tests/CMakeLists.txt`. Nothing was reported missing.
- I ran `pipeline.exe`. It saved `data/output/pipeline.png`. I opened the picture and confirmed that it shows the scatter plot, the histogram with bars, and both titles.
- After fixing `stats`, I ran all the tests with `ctest`. They all passed (100%), so the change did not break the existing tests.

## 6. Problems I faced and how I solved them

- **Linker error for `histogram_bin_edges`.** After I added `stats` to the pipeline, the build failed with `LNK2019: unresolved external symbol`. The function was written in `stats.cpp`, so it was not missing. The cause was that `stats.cpp` contained its own copy of the `summaryStatistics` class inside `namespace vizlib`, while `stats.hpp` declared a different class outside any namespace. The pipeline used the one from the header, which had no code behind it. I fixed this by keeping one class in `stats.hpp`, inside `namespace vizlib`, and making `stats.cpp` include that header.
- **`stats.hpp` had no `#pragma once`.** Every other header had one. Including it from two places would cause redefinition errors, so I added it.
- **Unsorted data.** `histogram_bin_edges()` and `quantile()` need sorted data, but the `y` values are noisy. I made the pipeline call `sorted()` first.
- **Duplicate lines in CMake.** `canvas.cpp` and `scatter.cpp` were listed twice in the root file, and `test_image_export.cpp` and `test_scatter.cpp` were listed twice in the tests file. I removed the extra lines.
- **`ecdf.cpp` needed a header that was not tracked by git.** After I changed its include to `<vizlib/ecdf.hpp>`, I made sure `include/vizlib/ecdf.hpp` was committed too, so a fresh clone of the branch can build.

## 7. Class structure

**Did I create any class?** No. This week I changed how an existing class is declared, but I did not create a new one. The pipeline itself is an ordinary program with a `main()` function, because it only runs the steps one after another and has nothing to store between calls.

My pipeline uses these types from the library:

**summaryStatistics** (from `stats.hpp`)

It stores a vector of numbers and provides functions for `mean`, `sorted`, `median`, `range`, `mode`, `standard_deviation`, `quantile`, `interquartile_range`, `count` and `histogram_bin_edges`. My pipeline uses `sorted()` and `histogram_bin_edges()`.

**TextRenderer** (from `text.hpp`)

It is created with the path to a font file. `measure_text()` returns the width and height of a piece of text, and `draw_text()` draws it on a canvas at a position, size and colour. Copying is disabled, but it can be moved. My pipeline uses `draw_text()` for the two titles.

**Canvas, Color and Rect** (from `canvas.hpp`, `color.hpp` and `geometry.hpp`)

The canvas holds the pixels, `Color` holds one pixel's colour, and `Rect` describes a plot area. My pipeline uses them to set up the picture and the two plot areas.

## 8. What is still pending

- Push the latest commit (`0dbae7e`) to GitHub and update pull request #61.
- The pipeline still uses generated data, because there is no CSV reader in the library yet. Once one exists, the data step should use it.
- The pipeline does not yet use `legend`, `annotations`, `figure`, `scale` or `ecdf`. The `legend` header only stores entries and has no drawing function yet.
- `stats`, `scale`, `axes`, `figure` and `ecdf` have no tests. Adding tests for `stats` would be the most useful next step.
- `ecdf.hpp` uses `namespace viz` and defines its own `Point`, while the rest of the library uses `namespace vizlib`. This should be agreed with the person who owns ECDF so the two `Point` types do not clash.
- The number of histogram bins is rounded down, so the last edge may fall slightly short of the largest value. This is worth checking with the owner of `stats`.
- The licence files for the vendored libraries (doctest, stb and DejaVu) are not kept in `third_party`.
