# Kabahamba Joy — Contribution Changelog

## Week 3

### Date
Week 3

### Changed
- Implemented text-rendering functionality using `TextRenderer`.
- Worked on annotation rendering on the Canvas.
- Implemented legend functionality and legend entry rendering.
- Integrated `Annotations` and `Legend` with the `Drawable` interface.

### Why
These changes provide the foundation for displaying text, annotations, and legends in visualisations, making plots easier to understand and interpret.

### PR
See the relevant Week 3 pull requests.

### Tested
- Tested text-rendering and drawing components.
- Verified integration with the library.

---

## Week 4

### Date
Week 4

### Changed
- Added figure title support.
- Added X-axis and Y-axis label support.
- Added setters and getters for figure titles and axis labels.
- Implemented `Figure::drawLabels()` using `TextRenderer`.
- Added tests for figure label functionality.
- Continued integration of `Drawable` with annotations and legends.

### Why
These changes improve figure presentation by providing titles and axis descriptions, while supporting consistent drawing behaviour across library components.

### PR
[Pull Request #94](https://github.com/kiwalabyeo/vizlib/pull/94)

### Tested
- Added tests for figure title and axis-label functionality.
- Tested that empty labels do not draw anything.
- Build and automated test checks passed.

---

## Week 5

### Date
2 November 2026

### Changed
- Completed and integrated text-rendering functionality using `TextRenderer` and `stb_truetype`.
- Added text measurement support for calculating text dimensions.
- Integrated figure titles and X-axis and Y-axis labels.
- Integrated tick-label rendering with the axes.
- Completed free-text annotation rendering.
- Completed legend entry rendering.
- Added the DejaVu Sans font under `third_party/fonts/`.
- Added module documentation, including `docs/text.md`.
- Added the integrated plotting example in `examples/pipeline.cpp`, demonstrating a scatter plot, histogram, figure labels, tick labels, an annotation, and a legend.

### Why
These changes complete the main text, labels, annotations, and legend functionality required for the visualisation library. They make plots more informative and improve the presentation and readability of generated figures.

### PR
To be linked when the Week 5 changes are submitted through a pull request.

### Tested
- Configured the project successfully with CMake and Ninja.
- Built the project successfully: all 59 build steps completed.
- Ran CTest successfully: 1/1 test suite passed, with 0 failures.
- Built and ran the pipeline example successfully.
- Confirmed that the example generated `data/output/pipeline.png`.
- Visual inspection of the generated image is still pending.
