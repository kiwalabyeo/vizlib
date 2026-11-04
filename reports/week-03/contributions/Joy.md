# Week 3 Contribution Report: Member 8, Text, Annotations and Legends

**Name:** Kabahamba Joy A Maria
**Task area:** Text rendering, annotations and legends
**Week:** 12 October to 18 October 2026, with the work merged from 19 to 25 October
**Pull requests:** #55, #62, #63, #64, #70

> **Note on this report.** The first version of this report described image export and build work, which is Member 7's module, not mine. On 4 November it was rewritten by Member 4 (Oscar), with Claude Code, to describe my own Week 3 work. The facts come from my pull requests on GitHub. The AI-use section is the one I wrote.

## 1. Work Completed

- **Text rendering (#55, merged 19 October).**
  - Added `TextRenderer` (`include/vizlib/text.hpp`, `src/text.cpp`). It loads a TrueType font once with stb_truetype, measures text (`measure_text`) and draws it on the Canvas (`draw_text`).
  - Added the DejaVu Sans font (`third_party/fonts/DejaVuSans.ttf`) and `third_party/stb_truetype.h`.
- **Annotations and legends (#55, then #62, #63 and #64, merged 25 October).**
  - Added `Annotations`, which stores notes (text, position, font size, colour) and draws them with `TextRenderer`.
  - Tidied the signature of `add_text`.
- **Legend drawing (#70, merged 25 October).** Each legend entry is drawn as a colour square with its label next to it.
- **Tests:** `tests/test_text.cpp`, `test_annotations.cpp` and `test_legend.cpp`, registered in `tests/CMakeLists.txt`.
- **Documentation:** `docs/text.md`, explaining how to use text, annotations and legends.

## 2. Challenges Encountered

- Local build errors in VS Code made some changes hard to verify.
- After the Week 3 merges, `main` did not build: `Annotations::draw` was declared twice, and `test_legend.cpp` was missing includes. Member 4 fixed both in #71.
- The work was opened after the week ended (19 to 25 October).

## 3. What came next (Week 4)

- Add a figure title and X and Y axis labels to `Figure`.
- Add a `Drawable` base class, so `Annotations` and `Legend` share one interface (inheritance and polymorphism).
- Draw tick labels on the axes.

## 4. Use of AI

AI assistance was used to understand build errors, investigate failed CI checks, review integration dependencies, and plan improvements.

## 5. Coordination with Group Members

- My text is drawn onto Member 6's shared `Canvas`.
- Member 4 fixed the build after the merges (#71).
