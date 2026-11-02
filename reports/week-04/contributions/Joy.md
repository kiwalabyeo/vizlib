# Week 4 Contribution Report — Kabahamba Joy

**Project:** vizlib — C++ Data Visualisation Library  
**Member:** 8 — Kabahamba Joy A Maria  
**Week:** 4  
**Area:** Text rendering, figure titles, axis labels, annotations and legends  
**Related pull request:** [PR #94](https://github.com/kiwalabyeo/vizlib/pull/94)

## 1. Work Completed

- Added figure title support to the `Figure` class.
- Added X-axis and Y-axis label support.
- Added setters and getters for figure titles and axis labels.
- Implemented `Figure::drawLabels()` using `TextRenderer`.
- Added unit tests for figure title and axis-label functionality.
- Registered `test_figure_labels.cpp` in `tests/CMakeLists.txt`.
- Continued integrating `Annotations` and `Legend` with the `Drawable` interface.

## 2. Implementation

The figure label drawing uses the existing text-rendering functionality. The `TextRenderer` handles text measurement and drawing, while the `Figure` class manages when and where the title and axis labels are drawn.

The X-axis and Y-axis labels make plotted data easier to interpret. The implementation also handles empty labels without drawing unnecessary text.

## 3. Testing and Verification

- Added tests for figure title and axis-label functionality.
- Tested that empty labels do not draw anything.
- Registered the new test file with the test build.
- The build and automated test checks passed for the related changes.

## 4. Outcome

The figure can now display a title and descriptive X-axis and Y-axis labels. This improves the readability of generated plots and builds on the text-rendering, annotation and legend work.

## 5. Next Steps

- Continue checking the integration of labels, annotations and legends with the plotting pipeline.
- Address any review feedback on the related changes.
