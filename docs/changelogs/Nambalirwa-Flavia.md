# Changelog: Member 6 (Nambalirwa Flavia Cate)

**Module:** Canvas rendering
**GitHub:** @FlaviaCate

<!-- Newest entries at the top. Add an entry in the same PR as the change. -->
<!-- Written on 5 November by Member 4 with Claude Code, from Flavia's commits and pull requests on GitHub. -->

## 2026-10-31
- **Changed:** Comments on every public function in `include/vizlib/canvas.hpp`.
- **Why:** So another programmer can use the Canvas without reading `canvas.cpp`.
- **PR:** #101, merged as #102
- **Tested:** Comments only. The build and tests pass.

## 2026-10-31
- **Changed:** Hardened the Canvas. NaN, infinity and coordinates bigger than 1,000,000 throw. A thickness must be from 1 to 1000. Lines fully past one edge are skipped. `fill_rect` and `fill_circle` only loop over the visible part. Empty rectangles draw nothing. `fill_circle` uses `long long` for the radius squared.
- **Why:** Bad or huge input could hang or crash a plot instead of giving a clear error.
- **PR:** #101, rebuilt on the current `main` by Member 4 as #102, keeping her commits
- **Tested:** `tests/test_canvas.cpp` grew to 19 test cases: thick lines, shapes off the canvas, one-pixel and empty rectangles, and bad input. All pass (GCC 13.3 in GitHub Codespaces).

## 2026-10-28
- **Changed:** The Canvas drawing code: `set_pixel`, `get_pixel`, `draw_line` with Bresenham's algorithm and a thickness, `draw_rect`, `fill_rect`, `draw_circle` and `fill_circle` with the midpoint algorithm. Also organised her Week 3 report.
- **Why:** Every plot, axis and piece of text is drawn onto the Canvas.
- **PR:** #81, rebuilt by Member 4 as #91 and merged on 29 October, keeping her commits
- **Tested:** First version of `tests/test_canvas.cpp`: pixels, lines, rectangles and circles.

## 2026-10-09
- **Changed:** Added `Color`, `Point`, `Rect` (`geometry.hpp`) and the `Canvas` header.
- **Why:** The shared types that every module uses, agreed before the code was written.
- **PR:** merged into `main` on 10 October, when the shared headers were agreed
- **Tested:** Headers only.
