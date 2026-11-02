# Week 4 Progress Report

**Week:** 19 October to 25 October 2026
**Who compiled the week's report:** Member 4 (Kiwalabye Oscar Muwanguzi)

Week 4 work was opened from 25 to 31 October and merged between 27 October and 1 November. Each item gives the pull request it came in.

## Completed
- **Joint plot (Member 4 Oscar, #73):**
  - A `Plot` base class with `ScatterPlot`, `HistogramPlot` and `KdePlot` deriving from it.
  - `JointPlot`, which holds three plots and draws them in three panels with shared scales.
  - `auto_bin_edges()` with Member 2's Freedman–Diaconis rule.
  - The scatter plot on iris, read with `read_csv()`. `iris.csv` was extended from 14 rows to the full 150.
  - `csv_reader.cpp`, `dataframe.cpp` and `stats.cpp` added to the build, plus 13 tests.
- **CI and the pipeline (Member 7 Norah, #74 and #80):** CI now runs `pipeline` and checks that `data/output/pipeline.png` exists and is not empty. The pipeline reads `iris.csv` instead of generated data, and gets its histogram bins from `summaryStatistics`.
- **Binning methods (Member 2 Melissa, #83):** a `binningStrategy` base class with `FreedmanDiaconisBinning` and `SturgesBinning`, so the binning method can be chosen through one interface.
- **ECDF (Member 3 Crystal, #85 to #88, merged together as #90):** tests, an example, and the move to `namespace vizlib`. Her changelog came in #84.
- **Text objects (Member 8 Joy, #89):**
  - A `Drawable` base class, with `Annotations` and `Legend` inheriting from it, and a test of runtime polymorphism.
  - Figure title and x and y axis labels in `Figure::drawLabels()`, with tests.
- **Canvas hardening (Member 6 Flavia, opened as #101 and merged as #102):**
  - Rejects NaN, infinity and huge coordinates.
  - Skips lines that cannot be visible.
  - Limits filled shapes to the canvas.
  - Tests for thick lines, off-canvas shapes and bad input.
- **Data loading (Member 1 James):** invalid-input handling (clear errors, with the line number for bad rows) and `CsvOptions` (delimiter, header, bad-row policy, NA values). Merged with the rest of his module in #108 on 1 November.

## In Progress
- Real KDE values for the joint plot: `KdePlot` draws the values it is given, and the example used a temporary KDE until Member 2's was ready (Members 2 and 4)
- Tick labels and axis lines on the plots (Members 5 and 8)
- Log scale for the axes (Member 5)
- Comments on the Canvas functions, `docs/canvas.md` and a shapes example (Member 6)

## Challenges/Blockers
- **Member 3's four ECDF pull requests did not build, alone or together.**
  - The header still used `namespace viz`.
  - The test file had its own `main()` and was not in CMake.
  - The example had a wrong include.

  Member 4 combined them as #90, keeping her commits, and fixed these.
- **Member 6's #81 came from an old branch** that carried commits already merged. It was rebuilt on the current `main` as #91 (Member 4).
- **Dependencies still missing at the end of the week.**
  - The KDE (Member 2) was not ready.
  - The Canvas drawing functions (Member 6) only arrived on 28 October.

  Member 4 kept temporary versions of both, marked to be replaced.
- **The 3-approval rule was not kept.** The repo owner merged with the override to get work onto `main` before the code freeze. Each merge was built and tested first (Member 4).
- **Missing contribution reports.** There are no Week 4 reports from Members 2, 3 and 5 (Members 2, 3, 5).

## Next Week
Week 5 is the final week, with the code freeze on Saturday 31 October.
- Lecturer's task: make the classes accept every numeric type with templates and an appropriate concept where it fits, rewrite the tests for them, and have each member document their templates and upload a UML sketch (All)
- KDE computation in the statistics module (Member 2), and the joint plot with KDE margins (Member 4)
- Tick labels on the axes (Member 8), and the log scale (Member 5)
- The complete data loading module, with tests and documentation (Member 1)
- README completed (Member 7)

## AI Use
- Tool: Claude
- Purpose: Member 4 used it (Claude Code) to design and draft the `Plot` hierarchy, the joint plot, tests and examples, to run the tests and sanitizers, and to draft his report. Member 6 used it to understand clipping, input validation and tests, and to draft code changes and test cases, which she reviewed, entered and tested herself
- Reason: To finish the joint plot despite missing dependencies, and to make the Canvas safe with bad input
- Used by: Member 4 (Oscar), Member 6 (Flavia)

- Tool: ChatGPT
- Purpose: Interpreting build and test output, Git branch and stash steps, and improving the report (Member 6). Preparing the report (Member 7)
- Reason: To fix build problems and write the reports
- Used by: Member 6 (Flavia), Member 7 (Norah)

- Tool: Gemini
- Purpose: As declared: `std::optional` interface design and the report structure
- Reason: To speed up header design and the report
- Used by: Member 1 (James)

- No AI section in the contribution report: Member 8 (Joy)
- No Week 4 contribution report: Member 2 (Melissa), Member 3 (Crystal), Member 5 (Destiny)
