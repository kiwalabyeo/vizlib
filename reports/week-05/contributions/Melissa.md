# Week 5 Contribution Report: Member 2, Statistics

**Name:** Natukunda Melissa, 25/U/0854
**Task area:** Statistical summaries, histogram binning and KDE
**Week:** 26 October to 1 November 2026, and the final checks up to 4 November
**Pull requests:** #104 (rebuilt as #107), #140

> **Note on this report.** Melissa had not handed in a Week 5 report by the submission day. On 5 November Member 4 (Oscar), with Claude Code, wrote this one from her pull requests on GitHub and from her Week 4 report. The AI section repeats what she declared herself.

## Completed

- **KDE (#104, 31 October).** Added `kde.hpp` and `kde.cpp`, which work out the kernel density estimate: a smooth curve of where the data is dense, made by adding a small Gaussian curve on every data point. The width of each curve comes from Silverman's rule for the bandwidth.
  - #104 conflicted with `main`, so Member 4 rebuilt it as #107, keeping her commit under her name. He also fixed `kde.hpp` so it compiled, added `kde.cpp` to CMake and wrote `tests/test_kde.cpp` (1 November).
  - `KdePlot` (Member 4) draws the curve, and `JointPlot::with_kde` uses it for the joint plot margins.
- **Module guide (#140, 4 November).** Wrote `docs/modules/stats.md`, explaining the summaries, the binning rules and the KDE, and updated her changelog and her part of the AI declaration.

## In Progress

- Nothing. The statistics module is complete.

## Challenges/Blockers

- The idea of a KDE was abstract at first. She read about it before using the code.
- The KDE pull request conflicted with `main` and its header did not compile, so it had to be rebuilt and fixed before it could be merged.

## Next Week

- Present the statistics part of the group presentation on 7 November.

## AI Use

- **Tool:** Claude
- **Purpose:** To help generate `kde.cpp` and `kde.hpp` (as she declared in her Week 4 report), and to draft her module guide.
- **Other sources:** Wikipedia and GeeksforGeeks, to understand KDE and the generated code.
