# Kabahamba Joy — Contribution Changelog

## Week 3

### Date
Week 3

### Changed
- Implemented text rendering functionality using `TextRenderer`.
- Worked on annotation rendering on the canvas.
- Implemented legend functionality and legend entry rendering.
- Integrated `Annotations` and `Legend` with the `Drawable` interface.

### Why
These changes allow the library to display text, annotations, and legends, making visualizations easier to understand and interpret.

### PR
See the relevant Week 3 pull requests.

### Tested
- Tested text rendering and drawing components.
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
