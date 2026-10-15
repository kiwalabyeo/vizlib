# Changelog: Kiwalabye Oscar Muwanguzi

**Module:** Joint plots (scatter, histogram, joint plot)
**GitHub:** @kiwalabyeo

<!-- Newest entries at the top. Add an entry in the same PR as the change. -->

## 15-10-2026
- **Changed:** Added `scatter()` and `ScatterStyle` (`include/vizlib/scatter.hpp`, `src/scatter.cpp`), with tests and `examples/scatter_example.cpp`. Added `canvas.cpp`, `scatter.cpp` and an examples option to the CMake build.
- **Why:** Week 2 goal: a scatter plot drawing on the Canvas with hand-picked points.
- **PR:** #51
- **Tested:** Added `tests/test_scatter.cpp` (5 test cases); all 9 test cases pass. Ran the example and checked the saved PNG.

## 02-10-2026
- **Changed:** Created the github repo, setup the rulestes and updated the file directory on the repo. Also added testing files to show the team how the whole thing will flow.
- **Why:** The repo allows version control and easy collaboration.
- **PR:** #1
- **Tested:** Added test files and shared PR links with the team.

- **Changed:** Added Continuos Integration, CMake, doctest and dummy test fil
- **Why:** To catch mistakes in code before they are pushed to main and to test on different systems that some teammate do not have e.g. code might run on windows but can't run on linux
- **PR:** #2
- **Tested:** Ran a test locally by changing 0.1.0 to 9.9.0 in tests/test_version.cpp.