# Changelog: Member 1 (Kizito Mark James)

**Module:** Data loading and preprocessing
**GitHub:** @kizitojamesss-ops

<!-- Newest entries at the top. Add an entry in the same PR as the change. -->
<!-- Written on 4 November by Member 4 with Claude Code, from James's pull requests on GitHub. -->

## 2026-11-05
- **Changed:** Member 4, with Claude Code, added `drop_nan(values)` to `preprocessing.hpp` and made the ECDF, rug plot, joint plot and statistics use it instead of their own copies. `pipeline` and three examples now use `drop_missing` and `df["name"]` instead of hand-made column helpers.
- **Why:** The data module was only used for reading files. Missing values were handled in four separate places, and one example helper could pair the wrong x and y.
- **PR:** the PR that adds this entry
- **Tested:** 2 new test cases (273 in total). Every example PNG is the same as before.

## 2026-11-04
- **Changed:** Member 4 extended `examples/data_examples.cpp` to show filtering, column selection and missing values, and wrote this changelog and `docs/modules/data.md`.
- **Why:** These functions worked, but no example showed them, and there was no changelog for Member 1.
- **PR:** #131, and the PR that adds this file
- **Tested:** `data_examples` runs, and the 49 data test cases pass.

## 2026-11-01
- **Changed:** Completed the data loading module.
  - Rewrote the CSV reader as a 4-state machine for quoted cells, `""` escapes, line breaks inside quotes, `\r\n` and the UTF-8 byte order mark.
  - Added `CsvOptions` and `BadRowPolicy`, the strict `parse_number()`, and the cleaning functions (`drop_missing`, `fill_missing`, `fill_missing_mean`, `fill_missing_median`, `to_numeric`, `filter_rows`, `filter_numeric`, `select_columns`, `drop_columns`).
  - Added `docs/data.md`, `data/input/messy.csv`, `examples/data_examples.cpp` and three test files.
- **Why:** The first version could not read quoted files such as `tips.csv`, and had no error handling or tests.
- **PR:** #105, rebuilt on the current `main` as #108 by Member 4, keeping my commits. Member 4 also added `preprocessing.cpp` and the test files to CMake.
- **Tested:** 49 test cases with 199 assertions, all passing, including every error path and the three sample files.

## 2026-10-24 to 2026-10-25
- **Changed:** First version of the data module: `Column`, `DataFrame`, a CSV reader, the cleaning functions, and the sample files `iris.csv` and `tips.csv`.
- **Why:** Every other module needs a `DataFrame` to read its numbers from.
- **PR:** #68, merged on 25 October
- **Tested:** Built in CI. The test files came later, in #108.
- **Reviews:** approved #60 (Member 5), #61 (Member 7), #62 and #64 (Member 8), #66 (Member 4) and #67 (Member 2).

## 2026-10-12
- **Changed:** Renamed my research file `KIZITO.md`.
- **PR:** #42, #43 and #44
- **Tested:** —
