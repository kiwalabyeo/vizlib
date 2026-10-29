# Week 4 Changelog

## Added
- Added figure title support to the `Figure` class.
- Added X-axis and Y-axis label support.
- Added setters and getters for figure titles and axis labels.
- Implemented `Figure::drawLabels()` to render titles and labels using `TextRenderer`.
- Added unit tests for figure label storage and rendering.
- Registered `test_figure_labels.cpp` in `tests/CMakeLists.txt`.

## Improved
- Continued integration of the `Drawable` interface with `Annotations` and `Legend`.
- Added runtime polymorphism test coverage.

## Testing
- Added tests for figure title and axis-label functionality.
- Added a test to verify that empty labels do not draw anything.
- Automated test results are pending confirmation through GitHub Actions.

## Notes
- The Y-axis label is currently rendered horizontally because rotated text rendering is not supported by the existing interface.

## Next Steps
- Verify all automated checks.
- Resolve any reported issues.
- Complete the required pull request review and merge process.
