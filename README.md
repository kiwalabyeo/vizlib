# vizlib

vizlib is a reusable C++20 library for visualising data. It reads a CSV file, cleans the data, computes statistics, and draws ECDF, rug, joint, scatter and histogram plots, which it saves as PNG images. Other programmers include its headers and call its functions, and small example programs show how.

Group 7, Programming project, Makerere University, 2026.

## Contents

- [Objectives](#objectives)
- [Main features](#main-features)
- [Project structure](#project-structure)
- [Requirements](#requirements)
- [Building](#building)
- [Using the library](#using-the-library)
- [Examples](#examples)
- [Running the tests](#running-the-tests)
- [Documentation](#documentation)
- [Limitations](#limitations)
- [Team members](#team-members)
- [Contributions](#contributions)
- [How we collaborated](#how-we-collaborated)
- [AI use](#ai-use)
- [License](#license)

## Objectives

### What the library sets out to do
- Load data: read CSV files into a DataFrame, and handle messy files (missing cells, quoted commas, bad rows).
- Prepare data: clean missing values, convert types, filter.
- Summarise data: mean, median, spread, quantiles, histogram bins, KDE.
- Visualise distributions: ECDF, rug and joint plots, plus the scatter and histogram they are built on.
- Map data to the image correctly: linear and log scales, readable ticks.
- Make plots readable: titles, axis labels, tick labels, legends.
- Produce real output: PNG files that open anywhere.
- Stay self-contained: no installs beyond a compiler and CMake, and build on Linux, Windows and macOS.
- Be reusable: a clean public API that someone else could drop into their own project.
- Be safe with bad input: clear exceptions instead of crashes.

### What the team sets out to learn and show
- Applying OOP in C++: classes, encapsulation, inheritance, polymorphism, and separating interface from implementation.
- Using templates and C++20 concepts where a class takes numeric data.
- Choosing data structures and algorithms (sorting for the ECDF, Bresenham for lines, KDE).
- Organising a professional project layout with CMake.
- Testing continuously with doctest and CI.
- Documenting so another programmer can use the library.
- Collaborating with Git: branches, pull requests and reviews.
- Developing iteratively, with weekly milestones and reports.
- Being able to explain and defend every design decision.

## Main features

- Reads CSV files into a DataFrame and handles messy data (missing cells, quoted commas, bad rows)
- Cleans data: missing values, type conversion, filtering, column selection
- Statistical summaries: mean, median, standard deviation, quantiles, IQR, histogram bins and KDE
- Plots: ECDF, rug, joint (with histogram or KDE margins), scatter and histogram
- Linear and log axis scaling, with readable ticks
- Axis lines, tick marks, tick numbers, titles, axis labels, legends and text annotations
- Saves plots as PNG (and PPM) images
- Builds on Linux, Windows and macOS with only a compiler and CMake

## Project structure

```
include/vizlib/   public headers, the only files users include
src/              implementation files
tests/            unit tests using doctest
examples/         small programs showing how to use the library
data/input/       sample CSV files (iris, tips, and a messy one)
data/output/      where the examples save their PNG images
docs/             documentation: module guides, design decisions, UML, changelogs, bug report
reports/          weekly progress reports, final report and AI use declaration
third_party/      doctest, stb_image_write, stb_truetype and the DejaVu Sans font
```

## Requirements

- A C++20 compiler (tested with GCC 13 on Ubuntu, MSVC on Windows and Apple Clang on macOS)
- CMake 3.16 or newer

Everything else is in `third_party/`, so there is nothing else to install.

## Building

```
git clone https://github.com/kiwalabyeo/vizlib.git
cd vizlib
cmake -S . -B build
cmake --build build
```

On Windows, run the same commands in the Developer Command Prompt.

## Using the library

Include the headers, read the data, draw into an `Axes` on a `Canvas`, then save the canvas as a PNG. This is a short version of `examples/scatter_example.cpp`. `examples/pipeline.cpp` is the full example.

```cpp
#include <vizlib/axes.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/csv_reader.hpp>
#include <vizlib/preprocessing.hpp>
#include <vizlib/scatter.hpp>
#include <vizlib/text.hpp>

int main()
{
    // 1. Read a CSV file and drop rows with missing values.
    const vizlib::DataFrame iris = vizlib::drop_missing(vizlib::read_csv("data/input/iris.csv"));
    const std::vector<double>& x = iris["sepal_length"];
    const std::vector<double>& y = iris["petal_length"];

    // 2. A white canvas, and a plot area with room for the tick numbers.
    vizlib::Canvas canvas(480, 360, vizlib::Color{255, 255, 255, 255});
    vizlib::Axes axes;
    axes.setPlotArea(vizlib::Rect{60, 40, 390, 260});

    // 3. Draw the scatter plot, then the axes and their tick numbers.
    vizlib::scatter(canvas, axes, x, y);
    const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
    axes.drawAxes(canvas, vizlib::Color{60, 60, 60, 255});
    axes.drawTickLabels(canvas, text);

    // 4. Save the picture as a PNG.
    return vizlib::save_png(canvas, "data/output/my_plot.png") ? 0 : 1;
}
```

To use vizlib in your own CMake project, add it as a subfolder and link to it:

```cmake
add_subdirectory(vizlib)
target_link_libraries(my_program PRIVATE vizlib)
```

## Examples

After building, the example programs are in `build/examples/`. Run them from the main `vizlib` folder so they can find the data files. Each one saves a PNG in `data/output/`.

- `pipeline`: the complete CSV-to-PNG pipeline: a scatter plot and a histogram with titles, labels, a legend and a note
- `ecdf_example`: ECDF and rug plot of iris petal length
- `scatter_example`: scatter plot
- `histogram_example`: histogram plot
- `joint_plot_example`: joint plot with histogram or KDE margins
- `stats_example`: summary statistics, the binning rules and a KDE curve
- `log_axis_example`: a plot on a logarithmic axis
- `shapes_example`: every Canvas shape (lines, rectangles, circles, filled shapes)
- `data_examples`: loading, cleaning, filtering and summarising data

On Windows:

```
.\build\examples\pipeline.exe
```

On Linux or macOS:

```
./build/examples/pipeline
```

## Running the tests

```
ctest --test-dir build --output-on-failure
```

On Windows, add the build type:

```
ctest --test-dir build -C Debug --output-on-failure
```

There are 271 test cases. GitHub Actions builds and tests every pull request on Linux, and every change to `main` on Linux, Windows and macOS. It also runs `pipeline` and checks that the PNG was created.

## Documentation

- `docs/modules/`: one guide per module: what it does, how it works, why it is written that way, how it is tested, and its limits
- `docs/design-decisions.md`: where templates, inheritance and other C++ features were used, and why
- `docs/uml/`: class diagrams
- `docs/data.md` and `docs/text.md`: full user guides for the data and text modules
- `docs/changelogs/`: what each member changed, and when
- `docs/bug-report.md`: the bugs found in the end-of-project testing, and how they were fixed
- `reports/`: weekly reports, the final report and the AI use declaration

## Limitations

- Plots are saved only as PNG or PPM images.
- Only numeric columns can be plotted.
- Text cannot be rotated, so the y-axis label is drawn horizontally above the axis.
- Lines and circles are not anti-aliased. Only text has smooth edges.
- The example programs must be run from the main `vizlib` folder, because they read `data/input/` and `third_party/fonts/DejaVuSans.ttf` using relative paths.

## Team members

- Member 1: Kizito Mark James, 25/U/30991/PS
- Member 2: Natukunda Melissa, 25/U/0854
- Member 3: Sengendo Crystal Zawedde, 25/U/08615/PS
- Member 4: Kiwalabye Oscar Muwanguzi, 25/U/08733/PS
- Member 5: Mugabe Destiny, 25/U/08748/PS
- Member 6: Nambalirwa Flavia Cate, 25/U/0848
- Member 7: Namaganda Norah Margret, 25/U/0846
- Member 8: Kabahamba Joy A Maria, 25/U/0830

## Contributions

Each name links to that member's commits on `main`.

- [Kizito Mark James](https://github.com/kiwalabyeo/vizlib/commits/main/?author=kizitojamesss-ops) (@kizitojamesss-ops): the Column and DataFrame classes, the CSV reader with bad-input handling, data cleaning (missing values, type conversion, filtering, column selection) and the sample CSV files.
- [Natukunda Melissa](https://github.com/kiwalabyeo/vizlib/commits/main/?author=melissanatukunda-xtian) (@melissanatukunda-xtian): the statistical summaries (mean, median, standard deviation, min, max, quantiles, IQR, mode), the histogram binning rules and the KDE.
- [Sengendo Crystal Zawedde](https://github.com/kiwalabyeo/vizlib/commits/main/?author=CrystalZawedde) (@CrystalZawedde): the ECDF plot, the rug plot and the ECDF example. She wrote the ECDF drawing and the rug plot herself on Member 4's laptop, using Claude Code, so that commit shows under his name (see `docs/modules/ecdf.md`).
- [Kiwalabye Oscar Muwanguzi](https://github.com/kiwalabyeo/vizlib/commits/main/?author=kiwalabyeo) (@kiwalabyeo): the scatter plot, the histogram plot, the joint plot with histogram or KDE margins, and their examples. Set up the repository, the CMake build, doctest and the first CI, the templates for pull requests, reports and changelogs, and the project plan. Coordinated the team, integrated the pull requests and ran the end-of-project testing.
- [Mugabe Destiny](https://github.com/kiwalabyeo/vizlib/commits/main/?author=destiny-mugabe) (@destiny-mugabe): linear and log axis scaling, tick generation, axis lines and tick marks, and the Figure and Axes layout.
- [Nambalirwa Flavia Cate](https://github.com/kiwalabyeo/vizlib/commits/main/?author=FlaviaCate) (@FlaviaCate): the Canvas drawing engine (lines, rectangles, circles, filled shapes, line thickness, clipping) and the shared Color, Point and Rect types.
- [Namaganda Norah Margret](https://github.com/kiwalabyeo/vizlib/commits/main/?author=NamagandaNorah-Margret20) (@NamagandaNorah-Margret20): PNG and PPM image export, the full CSV-to-PNG pipeline example, the CI step that runs the pipeline and checks the PNG, the numeric-type statistics tests, and this README's first full version and the LICENSE.
- [Kabahamba Joy A Maria](https://github.com/kiwalabyeo/vizlib/commits/main/?author=joyannaheaven3-afk) (@joyannaheaven3-afk): text rendering with stb_truetype, titles, axis labels, tick labels, legends, free-text annotations and the Drawable interface.

## How we collaborated

We split the work by module and agreed the header files first, so each of us could work without waiting on the others. Every change went through a pull request, and CI had to pass before it was merged. The plan was three approvals per pull request. Near the deadline that rule was not always kept: the repository owner merged some pull requests with the override after building and testing them together (see `reports/week-05/report.md`). A rotating scribe wrote each weekly report from everyone's changelog, and we coordinated in a group chat.

## AI use

AI tools were used for research, drafting and, at the end of the project, testing and fixing. Who used which tool, and for what, is in `reports/ai-use-declaration.md` and in the AI section of each weekly report. The end-of-project bug fixes are listed in `docs/bug-report.md`. This README was updated with Claude on 5 November, as allowed by the lecturer.

## License

MIT. See `LICENSE`.
