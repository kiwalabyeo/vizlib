### vizlib

vizlib description to be completed at the end of the project when we all know what exactly it does.

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
- 

## Project structure
include/vizlib/: public headers, the only files users include
src/: implementation files
tests/: unit tests using doctest
examples/: small programs showing how to use the library
reports/: weekly progress reports
docs/: detailed documentation for each module

## Requirements
A C++17 compiler (tested on GCC 13.3, Ubuntu, Windows and MacOS)
CMake 3.16 or newer

doctest is included in third_party/, so there's nothing else to install.

## Building
git clone https://github.com/kiwalabyeo/vizlib.git
cd vizlib
cmake -S . -B build
cmake --build build

On Windows, run the same commands in the Developer Command Prompt.

## Using the library


## Examples
- 

## Running the tests
cd build
ctest --output-on-failure

# On windows
cd build
ctest -c Debug --output-on-failure

All tests run automatically on every pull request through GitHub Actions.

## Limitations
- 

## Team members being referenced throughout the project
- Member 1: Kizito Mark James
- Member 2: Natukunda Melissa
- Member 3: Zawedde Crystal
- Member 4: Kiwalabye Oscar Muwanguzi
- Member 5: 
- Member 6:
- Member 7: Namaganda Norah Margret
- Member 8: Kabahamba Joy A Maria

## Contributions
- Kizito Mark James (@kizitojamesss-ops): built the Column and DataFrame classes, the CSV reader with bad-input handling, data cleaning (missing values, type conversion, filtering, column selection) and the sample CSV files.

- Natukunda Melissa (@melissanatukunda-xtian): wrote the statistical summaries (mean, median, standard deviation, min, max, quantiles, IQR, summary()), histogram binning and KDE.

- Zawedde Crystal (@CrystalZawedde): wrote the ECDF plot, the rug plot, and their examples.

- Kiwalabye Oscar Muwanguzi (@kiwalabyeo): wrote the scatter plot, the histogram plot, the joint plot combining them with histogram or KDE margins, and the joint plot example. Created the GitHub repo and its rules, the PR, report and changelog templates, and the project plan, and coordinated the team workflow.

- Member 5: wrote linear and log axis scaling, tick generation, and the Figure and Axes layout system.

- Member 6: built the Canvas rendering engine (lines, rectangles, circles, filled shapes, line thickness, clipping) and the shared Color, Point and Rect types.

- Namaganda Norah (@NamagandaNorah-Margret20): wrote PNG image export, set up the CMake build, the doctest test framework and GitHub Actions CI, and built the full pipeline example.

- Kabahamba Joy A Maria(@joyannaheaven3-afk): built text rendering with stb_truetype, plus titles, axis labels, tick labels, legends and free text annotations.
## How we collaborated

We split the work by module and agreed the header files first, so each of us could work without waiting on the other. Every change went through a pull request that needed three people's approval to merge to main. We have a rotating scribe and a grop chat for writing weekly reports based on every person's changelog.

## AI use

See the AI Use section in each weekly report in reports/.

## License

MIT. See LICENSE.