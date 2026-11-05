# Canvas Module Changelog

**Author:** Nambalirwa Flavia Cate (Member 6, Rendering / Canvas)
**Project:** vizlib, Group 7 (C++17)


Legend: **Added** = new, **Changed** = modified, **Fixed** = bug or build error corrected, **Not done** = planned but decided against

---

## Week 01 (to 4 October): Research

**Added**
- Research note (8 sections): module purpose, how lines, circles, thick lines and clipping work, needs from and for other members, roadmap, sources, AI use.
- Week 01 changelog 

**Decided**
- Keep the shape set to lines, rectangles and circles.
- Bresenham first; Wu's anti-aliased line only if time allowed.
- Bad input to throw `std::invalid_argument`, as the group agreed.

**Not done**
- No code this week.

---

## Week 02 (5 to 11 October): Shared types and Canvas interface

Branch: `feature/canvas-headers`

**Added**
- `include/vizlib/color.hpp`: `Color` (`unsigned char` r, g, b, a; default opaque black).
- `include/vizlib/geometry.hpp`: `Point` (x, y) and `Rect` (top-left x, y, width, height), both `double`.
- `include/vizlib/canvas.hpp`: `Canvas` class with private `width_`, `height_`, `std::vector<Color> pixels_`; public constructor, `width()`, `height()`, `set_pixel()`, `get_pixel()`.
- Week 02 report and change log.

**Fixed (after review)**
- Spelling of `Color`, an unclosed namespace, and constructor initialisation order.

**Setup**
- Cloned the repository on 9 October (after the 7 October interfaces deadline).
- Fixed git "Author identity unknown" with `git config --global user.name` and `user.email`.
- Fixed "not a git repository" by running commands inside the `vizlib` folder.

**Open questions raised**
- `int` vs `double` for `Point`/`Rect` (used `double`).
---

## Week 03 (12 to 18 October)

Branch: `feature/canvas-drawing`(merged with Branch: `feature/canvas-headers`, I mean I merged what was in Branch: `feature/canvas-headers`into canvas-drawing)

**Added** (`src/canvas.cpp`)
- Constructor: throws `std::invalid_argument` for width or height <= 0, fills pixels with the background colour.
- `set_pixel` (ignores off-canvas) and `get_pixel` (throws `std::out_of_range`).
- `draw_line`: Bresenham's algorithm, with `thickness` (default 1) drawn by stamping a square at each step; thickness < 1 rejected.
- `draw_rect` (outline) and `fill_rect`.
- `draw_circle` (midpoint algorithm, 8-way symmetry) and `fill_circle` (row scan); negative radius rejected.
- `tests/test_canvas.cpp` with the first tests (pixel access, lines, rectangles, circles, bad input).
- `test_canvas.cpp` registered in `tests/CMakeLists.txt`.
- Week 03 report and changelog.

**Fixed**
- `canvas.cpp` closed `namespace vizlib` too early and defined `fill_rect` twice; removed both problems and placed the anonymous namespace helpers correctly.

**Setup**
- CMake is not installed on my PC, so I built and tested in GitHub Codespaces (GCC 13.3.0).

**Verified**
- Build succeeded; `ctest` reported the `vizlib_tests` program passed.

**Decided**
- Do not prioritise Wu's algorithm; harden Bresenham instead `

---

## Week 04 (19 to 25 October)

**Added**
- `check_coord` helper: rejects NaN, infinity and |value| > 1,000,000 with `std::invalid_argument`, applied to coordinates, sizes and radii.
- `draw_line` thickness upper limit of 1000.
- `draw_line` early return when both endpoints are beyond the same edge (padding = thickness). This is a trivial reject, not full clipping.
- Empty rectangle rule: `draw_rect` and `fill_rect` do nothing for width or height <= 0 (a zero-height histogram bar is valid).
- Tests, growing the file to 19 test cases: thick lines, partly and fully off-canvas shapes and circles, one-pixel rectangle, empty rectangles, NaN/infinity/huge values.
- Week 04 report.
- Week 04 changelog 

**Changed**
- `fill_rect` and `fill_circle` loops clamped to the canvas.
- `fill_circle` radius-squared calculation uses `long long` to avoid overflow.

**Fixed**
- Build failure: added `#include <cmath>` to `tests/test_canvas.cpp` (`std::nan`, `INFINITY`).
- `fill_circle` was missing `check_coord`; added it plus a test line.
- Trailing whitespace on two lines of `canvas.cpp` (found with `git diff --check`).
- Edits made on `main`: moved with `git stash push`, `git switch feature/canvas-drawing`, `git stash pop`.

**Verified**
- GCC 13.3.0 build passed; `100% tests passed, 0 tests failed out of 1` (1 = registered CTest program).

**Not done**
- Wu's anti-aliased lines.

---

## Week 05 (26 to 31 October and 1st week of November code freeze)


- Public-function comments in `canvas.hpp` 
- `examples/shapes_example.cpp` (saves `data/output/shapes_example.png`) 
- README contribution paragraph 
- Final report (`docs/modules/canvas.md`)
- Week 05 changelog 