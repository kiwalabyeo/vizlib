# Week 1 Progress Report

**Week:** 28 September to 4 October 2026
**Who compiled the week's report:** Member 4 (Kiwalabye Oscar Muwanguzi)

## Completed
- Reviewed the original task split, rebalanced the work across all 8 members and wrote the project plan (Oscar)
- Created the GitHub repo with the required folder structure, .gitignore, LICENSE and a PR template (Oscar, #1)
- Set up a ruleset on main: pull requests required, 3 approvals, stale approvals dismissed, conversations must be resolved, no force pushes or deletions (Oscar)
- Added all 7 teammates as collaborators (Oscar)
- Added the CMake build, the doctest test framework, a version module with its first test, and the GitHub Actions CI workflow (Linux on every PR, Windows and macOS on pushes to main); build and tests pass locally and on CI (Oscar, #2, changelog in #3)
- Created templates for weekly reports and individual changelogs (Oscar)
- Added a CODEOWNERS file so reviewers are requested automatically and everyone gets an email when a PR opens (Oscar, #9)
- Merged the first README: objectives, project structure, requirements, build and test instructions, and planned contributions (Oscar, #10)
- 6 of 8 members merged their research files into docs/research/:
  - Data loading (Member 1, #7): CSV reading with `std::ifstream`, `std::stringstream` and `std::getline`; Members 2, 3, 4 and 7 depend on the DataFrame columns and CSV reader
  - Statistical summaries (Member 2, #4): mean, median, standard deviation, min, max, quartiles, IQR, a combined `summary()`, histogram binning and KDE, using only the standard library
  - ECDF plots (Member 3, #5): sort the data, compute F(x) = i/n for each value, and return step coordinates for the renderer; tests planned for empty, single-value and duplicate data
  - Axis scaling and layout (Member 5, #12): linear and log scales, mapping data to pixels and back, "nice" tick values, and Figure/Axes classes that support several Axes in one Figure for the joint plot; log scale rejects zero and negative values
  - Image export, build and integration (Member 7, #6): `save_png()` with stb_image_write, optional `save_ppm()` with no dependencies, maintaining CMake and CI, and the full CSV-to-PNG `pipeline.cpp` example
  - Labels and annotations (Member 8, #8): text drawing with stb_truetype and DejaVu Sans, plus titles, axis and tick labels, legends and free-text annotations, with text measurement for positioning

## In Progress
- Joint plots research (Member 4, #11): scatter, histogram (Freedman–Diaconis bins, vertical and horizontal) and a three-panel joint plot with shared scales and an optional KDE margin. PR is open with 2 of 3 approvals
- Rendering research (Member 6, #13): RGBA Canvas with Bresenham lines, midpoint circles, filled shapes and clipping, and the shared Color, Point and Rect types. Opened this week and merged on 6 October
- Making the CI build a required check on main, so no PR can merge with a failing build (Oscar)
- Agreeing the final library name; "vizlib" is the working name until then (All)
- README sections for features, usage, examples and limitations will be filled in as modules are completed (Oscar)

## Challenges/Blockers
- The first CMake setup failed three times: tests/ had no CMakeLists.txt, the doctest files were missing, and an include path did not match the folder name. All three are fixed, and the fixes are written up in the build guide so others avoid them (Oscar)
- The repo owner could still bypass the merge rules from the PR page. Research PRs #6, #7 and #8 were merged with 2 approvals instead of 3 using this bypass, to meet the Saturday deadline. Being fixed by removing all bypass permissions, so the rules apply to everyone from next week (Oscar)
- Each research PR needed 3 approvals, so 24 reviews were needed before Saturday. Members were asked to review the next three members' PRs, but most reviews came from Members 1, 3, 4 and 7. Members 2, 5 and 6 have not reviewed any PRs yet (All)
- Some first-time Git mistakes: files committed without the .md extension (fixed in #5 by renaming; `Axis_scaling` from #12 still has none), placeholder commit messages ("Your message"), and research file names that do not follow the `module-Name.md` pattern. A short Git checklist will be shared with the team (Oscar)

## Next Week
- Merge the interfaces PR (all public headers with agreed function signatures) by Wednesday 7 October (Members 1, 5, 6, 8)
- Settle open questions from the research in that PR: data loading covers CSV only (the research also covered GPU batching and JSON, which the library does not need); the ECDF header is named `ecdf.hpp`, and output is PNG only (the ECDF research also mentions SVG and PPM); the stats module computes KDE values and the joint plot only draws them (Members 1, 2, 3, 4)
- Working with tests: CSV reader (Member 1), basic statistics (Member 2), Canvas lines and rectangles (Member 6), linear scale (Member 5), PNG export (Member 7)
- Goal by Sunday 11 October: a test program draws a line on a Canvas and saves it as a PNG (Members 6, 7)
- Members 3, 4 and 8 start coding against the agreed interfaces (Members 3, 4, 8)
- Every member reviews at least 3 PRs, and Members 5 and 6 are added to CODEOWNERS so they get review requests (All, Oscar)
- Finalise the library name before the interfaces PR merges (All)

## AI Use
- Tool: Claude
- Purpose: Reviewing the original task split, planning the repo structure and workflow, and explaining and providing starting configuration for CMake, doctest and GitHub Actions, which was tested locally and adapted. Also used for the report and changelog templates, guidance on README structure (the README itself is written by the team), and explaining joint plots, histogram binning and KDE, and drafting the joint plots research file, which was reviewed and edited. Member 6 used it to explain Canvas drawing algorithms and to tidy the rendering research
- Reason: To check the workload was balanced, to save time on first-time C++ CI setup, and to understand modules quickly before writing the research
- Used by: Member 4 (Oscar), Member 6 (Flavia)

- Tool: Gemini
- Purpose: Supporting research on how to implement each module, for example efficient C++ data structures and CSV parsing, and structuring research files to match the group plan
- Reason: To speed up research and understand unfamiliar topics before writing the research files
- Used by: Member 1 (James), Member 2 (Melissa), Member 6 (Flavia)

- Tool: ChatGPT
- Purpose: Understanding module requirements and dependencies (axis scaling and ticks, CMake, CI and stb_image_write, stb_truetype and text layout) and organising the research files and roadmaps
- Reason: To learn the topic and plan the work before coding
- Used by: Member 5 (Destiny), Member 7 (Norah), Member 8 (Joy)

- No AI use declared: Member 3 (Crystal)
