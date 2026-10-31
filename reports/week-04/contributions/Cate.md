# Week 04 Contribution Report: Member 6 – Canvas Rendering

**Name:** Nambalirwa Flavia Cate  
**Registration Number:** 25/U/0848  
**Programme:** BELE  
**Group:** 7 – vizlib  
**Branch:** `feature/canvas-drawing`  


## Completed

- **Improved input safety in the Canvas implementation.** I updated `src/canvas.cpp` to validate drawing inputs before using them. The `check_coord` helper rejects non-finite values (NaN and infinity) and values whose absolute magnitude exceeds 1,000,000 by throwing `std::invalid_argument`. The drawing methods apply these checks to their relevant coordinates, dimensions and radii. `draw_line` also rejects a thickness smaller than 1 or greater than 1,000.

- **Added a quick rejection check for lines that cannot be visible.** `draw_line` now returns early when both endpoints are sufficiently far beyond the same canvas edge. The check accounts for line thickness. This avoids walking through a long sequence of pixels when the line cannot reach the canvas. Lines that may cross the canvas are still processed by the existing Bresenham line-drawing loop.

- **Restricted filled-shape loops to the canvas.** `fill_rect` and `fill_circle` limit their drawing loops to the visible canvas area. This prevents a large off-canvas portion of a filled shape from causing unnecessary iterations. `fill_circle` also uses `long long` for the radius-squared calculation to avoid integer overflow in that calculation.

- **Defined how empty rectangles are handled.** `draw_rect` and `fill_rect` do not draw anything when the rectangle has a non-positive width or height. I chose this behaviour because a zero-sized shape can be a valid no-op in plotting; it should not necessarily stop the whole plot by throwing an exception.

- **Expanded the Canvas tests.** I added tests in `tests/test_canvas.cpp` for thick lines, shapes that are partly or fully outside the canvas, large off-canvas circles, one-pixel rectangles, empty rectangles, and invalid inputs such as NaN, infinity and values outside the accepted coordinate range.

- **Built and tested the project in GitHub Codespaces.** I configured the project with CMake using GCC 13.3.0, built it successfully, and ran `ctest --test-dir build --output-on-failure`. The final result was `100% tests passed, 0 tests failed out of 1`. The `1/1` refers to the registered CTest test program, not the number of individual test cases.

## In Progress

- Adding comments to the public functions in `include/vizlib/canvas.hpp`, describing their parameters, behaviour and possible exceptions.
- Writing `docs/canvas.md` to document the Canvas module.
- Preparing `examples/shapes_example.cpp` to draw shapes.

## Challenges and Lessons Learned

- **Understanding the difference between rejecting and clipping a line.** The new early-return check avoids processing a line that is clearly outside the canvas. However, it is not a complete geometric line-clipping algorithm. A line that may cross the canvas is still processed by Bresenham's algorithm, and out-of-range pixels are ignored by the canvas bounds handling. Exact clipping to the canvas boundary remains a possible future improvement.

- **Choosing behaviour for empty rectangles.** I considered whether a rectangle with a non-positive width or height should throw an exception. I chose to treat it as a no-op so that zero-sized shapes do not unnecessarily interrupt plotting. This behaviour will also be documented.

- **Missing header during the first build.** The first Codespaces build failed because the new tests used `std::nan()` and `INFINITY` without including `<cmath>`. I added `#include <cmath>` to `tests/test_canvas.cpp`, rebuilt the project, and the build succeeded. This reminded me to check the required standard-library headers when adding tests.

- **Moving uncommitted changes to the correct branch.** I noticed that I had made the edits while on `main` in Codespaces. Before committing, I used `git stash` to save the uncommitted changes, switched to `feature/canvas-drawing`, and used `git stash pop` to restore them. I then confirmed that the edits were on the feature branch.

## Next Week

- Complete the public function comments, `docs/canvas.md` and `examples/shapes_example.cpp`.
- Write my own contribution paragraph for the README Contributions section, as required by the group instructions.
- Check that the other modules use the Canvas interface correctly and address any integration issues found.
- Continue with the existing Bresenham line-drawing approach. Xiaolin Wu's antialiased line algorithm was optional, so I am not planning to implement it at this stage.

## AI Use Declaration

- **Claude (Anthropic):** I used Claude to help me understand clipping, input validation and the tests, and to draft code changes and test cases. I reviewed the suggestions, entered the changes, built and tested the project, and checked that I could explain the changes.
- **ChatGPT (OpenAI):** I used ChatGPT to help interpret the Codespaces build and test output, understand the missing `<cmath>` header and whitespace-check messages, work through the Git branch and stash steps, and review and improve this report. I applied the changes and ran the commands in Codespaces myself.
