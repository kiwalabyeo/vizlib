# Week 1 Progress Report

**Week:** 28 September to 4 October 2026
**Who compiled the week's report:** Kiwalabye Oscar Muwanguzi

## Completed
- Reviewed the original task split, rebalanced the work across all 8 members and wrote the project plan (Oscar)
- Created the GitHub repo with the required folder structure, .gitignore, LICENSE and a PR template (Oscar)
- Set up a ruleset on main: pull requests required, 3 approvals, stale approvals dismissed, conversations must be resolved, no force pushes or deletions (Oscar)
- Added all 7 teammates as collaborators (Oscar)
- Added the CMake build, the doctest test framework, a version module with its first test, and the GitHub Actions CI workflow; build and tests pass locally and on CI (Member 7, #3)
- Created templates for weekly reports and individual changelogs (Oscar)
- All 8 members researched their modules and merged their research files into docs/research/ (Members 1-8, [#PR numbers])

## In Progress
- Making the CI build a required check on main, so no PR can merge with a failing build (Oscar)
- Agreeing the final library name; "vizlib" is the working name until then (All)
- [README structure drafted; sections to be filled in as modules are completed (Oscar)]

## Challenges/Blockers
- First CMake setup failed three times (missing CMakeLists.txt in tests/, missing doctest files, and an include path that did not match the folder name). All fixed, and the fixes are written up in the build guide so others avoid them (Member 7)
- The repo owner could still bypass the merge rules from the PR page. Being fixed by removing all bypass permissions, so the rules apply to everyone (Oscar)
- Each research PR needed 3 approvals, so 24 reviews were needed before Saturday. Handled by asking each member to review the next three members' PRs ([All / say what actually happened])
- [Any member who had trouble installing Git, accepting the invite or opening a PR, and how it was solved]

## Next Week
- Merge the interfaces PR (all public headers with agreed function signatures) by Wednesday 7 October (Members 1, 5, 6, 8)
- Working with tests: CSV reader (Member 1), basic statistics (Member 2), Canvas lines and rectangles (Member 6), linear scale (Member 5), PNG export (Member 7)
- Goal by Sunday 11 October: a test program draws a line on a Canvas and saves it as a PNG (Members 6, 7)
- Members 3, 4 and 8 start coding against the agreed interfaces (Members 3, 4, 8)
- Finalise the library name before the interfaces PR merges (All)

## AI Use
- Tool: Claude
- Purpose: Reviewing the original task split, planning the repo structure and workflow, and explaining and providing starting configuration for CMake, doctest and GitHub Actions, which was tested locally and adapted. Also used for report and changelog templates and guidance on README structure (the README itself is written by the team)
- Reason: To check the workload was balanced, and to save time on first-time C++ CI setup
- Used by: Oscar

- Tool: Gemini
- Purpose: Supporting research on how to implement each module, e.g. algorithms and suitable C++ libraries
- Reason: To speed up research and understand unfamiliar topics before writing the research files
- Used by: [names or member numbers of everyone who used it]
- Not used by: [anyone who did their research without AI]