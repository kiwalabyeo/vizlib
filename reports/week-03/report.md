# Week 3 Progress Report

**Week:** 12 October to 18 October 2026
**Who compiled the week's report:** Member 4 (Kiwalabye Oscar Muwanguzi)

Most Week 3 code was opened from 19 to 25 October, after the week ended, and merged between 19 and 29 October. Each item below gives the pull request it came in.

## Completed
- Histogram in vertical and horizontal orientation, `histogram_counts()`, 8 tests and `histogram_example.cpp`. Also added `scale.cpp`, `axes.cpp` and `figure.cpp` to the build, because they were on `main` but not compiled (Member 4 Oscar, #57). The Week 2 scatter plot was merged on 15 October (#51)
- ECDF coordinate calculation in `ecdf.cpp`: sorts the data, works out y = (i + 1) / N, and returns an empty result for empty data (Member 3 Crystal, #53)
- Statistics files moved into `include/` and `src/`, and the Freedman–Diaconis `histogram_bin_edges()` added (Member 2 Melissa, #54). Further binning fixes were merged in #72
- Text rendering with stb_truetype and DejaVu Sans (`TextRenderer`, with text measurement), plus `Annotations` and `Legend` with tests and `docs/text.md` (Member 8 Joy, #55). Annotation and legend drawing followed in #62, #63, #64 and #70
- Tick generation: `generateTicks()` picks readable steps (1, 2 or 5 times a power of ten) and rejects invalid ranges (Member 5 Destiny, #60)
- Canvas export adapter: `save_png()` and `save_ppm()` for a whole Canvas (Member 7 Norah, #61). First `pipeline.cpp`, which draws a scatter plot and a histogram side by side with titles and saves one PNG, plus a CMake clean-up: duplicate source and test lines removed, and `stats.cpp` and `ecdf.cpp` added (Member 7 Norah, #74)
- First data loading module and the iris and tips sample datasets (Member 1 James, #68). The rewritten reader described in his Week 3 report (quoted cells, `parse_number()`, tests) was merged later, in #108
- Canvas drawing: `draw_line()` (Bresenham), `draw_rect()`, `fill_rect()`, `draw_circle()` and `fill_circle()` (midpoint circle), with tests (Member 6 Flavia, opened as #81 on 28 October and merged as #91)
- Week 3 assignment: the report on encapsulation, inheritance and polymorphism in vizlib, with hand-drawn class figures, in `reports/week-03/assignment/` (Member 7 Norah, #77, #78, #79)

## In Progress
- Scatter plot on real CSV data: blocked until the CSV reader was built on `main`. Done in Week 4 (Member 4)
- ECDF unit tests and an example (Member 3)
- Making the data loading module handle real files and bad input (Member 1)
- Figure title and axis labels (Member 8)

## Challenges/Blockers
- **Work arrived late.**
  - Nearly all Week 3 pull requests were opened after the Saturday deadline, most of them on 19, 20 and 25 October.
  - Member 6's drawing functions only arrived on 28 October. In the meantime Member 4 used temporary drawing helpers, and a simple `Canvas::fill_rect` was added in #71 so that the legend could draw.
- **Merging several pull requests together on 25 October broke `main`.** It was fixed the same day in #71 (Member 4):
  - `Annotations::draw` was declared twice.
  - `test_legend.cpp` had lost its includes.
  - `Canvas::fill_rect` was declared but never implemented.
- **Some pull requests conflicted with `main` and were rebuilt by Member 4 on the current `main`, keeping the author's commits:**
  - #65 (annotation rendering) was already covered by #62 to #64 and #70, and its build fixes went into #71.
  - #67 (binning) was rebuilt as #72.
- **Duplicate pull requests.** The same tick generation work was opened four times (#56, #58, #59, #60). Only #60 was merged (Member 5).
- **Code on `main` that was not compiled.** Several `.cpp` files were on `main` but not in `CMakeLists.txt`, so CI could not catch their errors. Members 4 and 7 added them. Everyone was reminded to add new files to CMake in the same pull request (All).
- **The 3-approval rule was not kept.** The repo owner merged several pull requests without three approvals to keep `main` moving (Member 4).
- **One contribution report describes another member's work.** Member 8's Week 3 report described image export and build work, which is Member 7's module. On 4 November Member 4 rewrote it from her pull requests to describe her own text, annotation and legend work.

## Next Week
- Joint plot: scatter in the centre with histogram or KDE margins and shared scales, a `Plot` base class for inheritance and polymorphism, and the scatter plot on iris (Member 4)
- CI runs the pipeline and checks the PNG, and the pipeline reads `iris.csv` instead of generated data (Member 7)
- Several binning methods behind one interface (Member 2)
- ECDF tests and example, in `namespace vizlib` (Member 3)
- Invalid-input handling and options for the CSV reader (Member 1)
- Figure title and axis labels, and a `Drawable` base class for text objects (Member 8)
- Canvas input validation and clipping (Member 6)
- Log scale for the axes (Member 5)

## AI Use
- Tool: Claude
- Purpose: Member 4 used it (Claude Code) to check which dependencies were missing, draft the histogram, its tests and example, and draft his report. Member 2 used it to understand encapsulation in the statistics class. Member 6 used it, together with ChatGPT, to understand and improve parts of the Canvas code and to organise her report
- Reason: To work around missing dependencies, understand OOP concepts, and write clearer code and reports
- Used by: Member 4 (Oscar), Member 2 (Melissa), Member 6 (Flavia)

- Tool: Gemini
- Purpose: Debugging C++ syntax and type-casting errors and learning `static_cast` (Member 3). Header design with `std::optional` and drafting the report structure (Member 1)
- Reason: To fix errors faster and follow standard C++ practice
- Used by: Member 3 (Crystal), Member 1 (James)

- Tool: AI assistant, not named in the report
- Purpose: Understanding header and source file organisation, the maths of tick generation and OOP concepts (Member 5). Understanding build errors and failed CI checks (Member 8)
- Reason: To understand the work before and while writing it
- Used by: Member 5 (Destiny), Member 8 (Joy)

- No AI section in the contribution report: Member 7 (Norah)
