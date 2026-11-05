# Week 5 Contribution Report: Member 6, Canvas Rendering

**Name:** Nambalirwa Flavia Cate, 25/U/0848
**Task area:** Canvas rendering and the shared Color, Point and Rect types
**Week:** 26 October to 1 November 2026
**Pull requests:** #101, rebuilt by Member 4 as #102

## Completed

- **A safer Canvas (#101, 31 October).**
  - NaN, infinity and coordinates bigger than 1,000,000 now throw `std::invalid_argument`. A line thickness must be between 1 and 1000.
  - A line whose two ends are past the same edge is skipped, and the filled shapes only loop over the visible part of the canvas.
  - Empty rectangles draw nothing, because a histogram bar of height 0 is valid.
  - New tests for thick lines, shapes off the canvas, one-pixel and empty rectangles, and bad input. `tests/test_canvas.cpp` now has 19 test cases.
  - #101 conflicted with `main`, so Member 4 rebuilt it on the current `main` as #102, keeping her commits under her name. It was merged on 31 October.
- **Comments (in #102).** Comments on every public function in `canvas.hpp`, saying what it does, what it takes and what it throws.

## In Progress

- Nothing. The Canvas module is complete. Member 4 added `shapes_example` on 4 November, and wrote her module guide `docs/modules/canvas.md` on 5 November.

## Challenges/Blockers

- Her pull request conflicted with other work merged in the same week, so it could not be merged as it was.

## Next Week

- Present the Canvas part of the group presentation on 7 November.

## AI Use

- No AI use was recorded for Week 5. Her declarations for earlier weeks are in `reports/ai-use-declaration.md`.
