### vizlib

vizlib is a reusable C++20 library for visualising data. It reads a CSV file, cleans the data, computes statistics, and draws ECDF, rug, joint, scatter and histogram plots, which it saves as PNG images. Other programmers include its headers and call its functions, and small example programs show how.

## Objectives
# What the library sets out to do
- Load data by read CSV files into a DataFrame, and handle messy files (missing cells, quoted commas, bad rows).
- Prepare data by clean missing values, convert types, filter.
- Summarise data using mean, median, spread, quantiles, histogram bins, KDE.
- Visualise distributions through ECDF, rug and joint plots, plus the scatter and histogram they're built on.
- Map data to the image correctly: linear and log scales, readable ticks.
- Make plots readable: titles, axis labels, tick labels, legends.
- Produce real output: PNG files that open anywhere.
- Stay self-contained: no installs beyond a compiler and CMake, and it builds on Linux, Windows and macOS.
- Be reusable: a clean public API that someone else could drop into their own project.
- Be safe with bad input: clear exceptions instead of crashes.

# What the team sets out to learn and show
- Applying OOP in C++: classes, encapsulation, separating interface from implementation.
- Choosing data structures and algorithms (sorting for ECDF, Bresenham for lines, KDE).
- Organising a professional project layout with CMake.
- Testing continuously with doctest and CI.
- Documenting so another programmer can use the library.
- Collaborating with Git: branches, PRs, 3-person reviews.
- Developing iteratively, with weekly milestones and reports.
- Being able to explain and defend every design decision.

## Main features

- Reads CSV files into a DataFrame and handles messy data (missing cells, quoted commas, bad rows)
- Cleans data: missing values, type conversion, filtering, column selection
- Statistical summaries: mean, median, standard deviation, quantiles, IQR, histogram bins and KDE
- Plots: ECDF, rug, joint, scatter and histogram
- Linear and log axis scaling, with readable ticks
- Titles, axis labels, tick labels, legends and text annotations
- Saves plots as PNG images
- Builds on Linux, Windows and macOS with only a compiler and CMake

## Project structure
include/vizlib/: public headers, the only files users include
src/: implementation files
tests/: unit tests using doctest
examples/: small programs showing how to use the library
reports/: weekly progress reports
docs/: detailed documentation for each module

## Requirements
A C++20 compiler (tested on GCC 13.3, Ubuntu, Windows and MacOS)
CMake 3.16 or newer

doctest is included in third_party/, so there's nothing else to install.

## Building
git clone https://github.com/kiwalabyeo/vizlib.git
cd vizlib
cmake -S . -B build
cmake --build build

On Windows, run the same commands in the Developer Command Prompt.

## Using the library

Include the headers, create a canvas and a figure, call a plot function, then save the canvas as a PNG. This is a short version of `examples/pipeline.cpp`, which is the full working example.

```cpp
#include <vizlib/figure.hpp>
#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/scatter.hpp>
#include "vizlib/csv_reader.hpp"

#include <variant>
#include <vector>

int main()
{
    // 1. Read a CSV file into a DataFrame.
    const vizlib::DataFrame df = vizlib::read_csv("data/input/iris.csv");

    // 2. Copy two columns into lists of numbers.
    std::vector<double> x, y;
    for (const vizlib::CellValue& cell : df.get_column("sepal_length"))
        if (const double* n = std::get_if<double>(&cell)) x.push_back(*n);
    for (const vizlib::CellValue& cell : df.get_column("petal_length"))
        if (const double* n = std::get_if<double>(&cell)) y.push_back(*n);

    // 3. Create a canvas (the picture) and a figure with one plot area.
    const vizlib::Color white{255, 255, 255, 255};
    vizlib::Canvas canvas(400, 300, white);
    vizlib::Figure figure(400, 300);
    vizlib::Axes& axes = figure.addAxes(vizlib::Rect{0, 0, 400, 300});

    // 4. Draw the scatter plot.
    vizlib::scatter(canvas, axes, x, y);

    // 5. Save the picture as a PNG.
    vizlib::save_png(canvas, "data/output/my_plot.png");
    return 0;
}
```

## Examples
After building, the example programs are in `build/examples/`. Run them from the main `vizlib` folder so they can find the data files:

- `pipeline`: the complete CSV-to-PNG pipeline (saves `data/output/pipeline.png`)
- `ecdf_example`: ECDF and rug plots
- `scatter_example`: scatter plot
- `histogram_example`: histogram plot
- `joint_plot_example`: joint plot with histogram or KDE margins
- `data_examples`: loading, cleaning and summarising data

For example, on Windows:

    .\build\examples\pipeline.exe

On Linux or macOS:

    ./build/examples/pipeline

## Running the tests
cd build
ctest --output-on-failure

# On windows
cd build
ctest -C Debug --output-on-failure

All tests run automatically on every pull request through GitHub Actions.

## Limitations

- Plots are saved only as PNG images.
- Only numeric columns can be plotted. Text cells are skipped when a column is converted to numbers.
- The example programs must be run from the main `vizlib` folder, because they read `data/input/` and `third_party/fonts/DejaVuSans.ttf` using relative paths.

## Team members being referenced throughout the project
- Member 1: Kizito Mark James
- Member 2: Natukunda Melissa
- Member 3: Zawedde Crystal
- Member 4: Kiwalabye Oscar Muwanguzi
- Member 5: Mugabe Destiny
- Member 6: Nambalirwa Flavia Cate
- Member 7: Namaganda Norah Margret
- Member 8: Kabahamba Joy A Maria

## Contributions
- Kizito Mark James (@kizitojamesss-ops): built the Column and DataFrame classes, the CSV reader with bad-input handling, data cleaning (missing values, type conversion, filtering, column selection) and the sample CSV files.

- Natukunda Melissa (@melissanatukunda-xtian): wrote the statistical summaries (mean, median, standard deviation, min, max, quantiles, IQR, summary()), histogram binning and KDE.

- Zawedde Crystal (@CrystalZawedde): wrote the ECDF plot, the rug plot, and their examples.

- Kiwalabye Oscar Muwanguzi (@kiwalabyeo): wrote the scatter plot, the histogram plot, the joint plot combining them with histogram or KDE margins, and the joint plot example. Created the GitHub repo and its rules, the PR, report and changelog templates, and the project plan, and coordinated the team workflow.

- Mugabe Destiny (@destiny-mugabe): wrote linear and log axis scaling, tick generation, and the Figure and Axes layout system.

- Nambalirwa Flavia Cate(@FlaviaCate): built the Canvas rendering engine (lines, rectangles, circles, filled shapes, line thickness, clipping) and the shared Color, Point and Rect types.

- Namaganda Norah (@NamagandaNorah-Margret20): wrote PNG image export, set up the CMake build, the doctest test framework and GitHub Actions CI, and built the full pipeline example.

- Kabahamba Joy A Maria(@joyannaheaven3-afk): built text rendering with stb_truetype, plus titles, axis labels, tick labels, legends and free text annotations.
## How we collaborated

We split the work by module and agreed the header files first, so each of us could work without waiting on the other. Every change went through a pull request that needed three people's approval to merge to main. We have a rotating scribe and a group chat for writing weekly reports based on every person's changelog.

## AI use

See the AI Use section in each weekly report in reports/.

## License

MIT. See LICENSE.
