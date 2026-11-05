# Data: reading CSV files and cleaning them

- **Owner:** Kizito Mark James (Member 1)
- **Headers:** `include/vizlib/dataframe.hpp`, `csv_reader.hpp`, `preprocessing.hpp`
- **Sources:** `src/dataframe.cpp`, `src/csv_reader.cpp`, `src/preprocessing.cpp`
- **Tests:** `tests/tests_dataframe.cpp`, `test_csv_reader.cpp` and `test_preprocessor.cpp` and `test_numeric_types.cpp` (74 test cases)
- **Example:** `data_examples`, and `pipeline`, which reads `data/input/iris.csv`
- **User guide:** `docs/data.md` (every function and every error)

## 1. What it does

It is the first step of the pipeline. It reads a CSV file into a `DataFrame` (a table of named columns), and cleans it. Every other module takes its numbers from here.

```cpp
#include <vizlib/csv_reader.hpp>
#include <vizlib/preprocessing.hpp>

auto df = vizlib::read_csv("data/input/tips.csv");
df = vizlib::drop_missing(df);
const std::vector<double>& bill = df["total_bill"];   // ready for ECDFPlot or summaryStatistics
```

## 2. How it works

**Reading a file.** `read_csv` goes through the text one character at a time with a small state machine. It has 4 states:

1. `FieldStart`: at the start of a cell.
2. `Unquoted`: inside a normal cell. A comma ends the cell, a line break ends the row.
3. `Quoted`: inside `"..."`. Commas and line breaks are kept as text.
4. `AfterQuote`: just after a closing quote. A second `"` means a real quote character (`""`), so it goes back to `Quoted`.

Before it starts, it skips the UTF-8 byte order mark that Excel adds. It accepts both `\r\n` (Windows) and `\n` line endings. If the file ends while still in `Quoted`, it throws "unterminated quoted field" with the line number.

**Numbers or text.** After reading, each column is checked. If every cell that is not missing is a plain number (`12`, `-3.5`, `2e3`), the column becomes numeric (`double`). Otherwise it stays text. `parse_number` is strict: `inf`, `nan`, `0x1A` and numbers too big for a `double` are not numbers. Empty cells and words like `NA` or `null` become missing. In a numeric column a missing cell is `NaN`.

**Cleaning.** `drop_missing` removes rows with a missing cell. `fill_missing`, `fill_missing_mean` and `fill_missing_median` fill the gaps. `to_numeric` turns a text column into numbers. `filter_rows`, `select_columns` and `drop_columns` keep only the rows or columns you want. Each one returns a **new** `DataFrame`, so the original is never changed.

**`drop_nan`.** It takes a plain list of numbers and leaves out the NaN values, in the same order. The rest of the library uses it: the ECDF and rug plot, the joint plot's KDE margins and the statistics all call it, so missing values are handled in one place.

**Where the module is used.** `pipeline` and the scatter, ECDF and joint plot examples all start with `read_csv`, then `drop_missing`, then take each column with `df["name"]`. The plots themselves take `std::vector<double>`, so they also work with numbers that did not come from a CSV file.

## 3. Why it is written this way

- **Classes and encapsulation.** `Column` and `DataFrame` keep their data private. `add_column` throws if the name is already used or the length is wrong, so a `DataFrame` always has columns of the same length.
- **`enum class`.** `Column::Type` (`Numeric` or `Text`), `BadRowPolicy` (`Throw` or `Skip`) and the reader's `State` are `enum class`, so they cannot be mixed up with plain numbers.
- **A conversion operator.** A `Column` turns into `const std::vector<double>&` by itself. That is why `vizlib::ECDFPlot(df["tip"])` works with no extra step. It returns a reference, so nothing is copied.
- **`const` and references.** The cleaning functions take `const DataFrame&` and return a new one, the same as pandas.
- **Exceptions with line numbers.** A bad file throws `std::invalid_argument` with the line number in the message. A file that cannot be opened throws `std::runtime_error`.
- **Options in a struct.** `CsvOptions` holds the delimiter, the header flag, the bad-row policy and the missing-value words, with defaults. So `read_csv(path)` works with no options.
- **No templates.** CSV cells are always text or `double`, so a template would add nothing.
- **Old names still work.** `row_count()`, `get_column()` and `handle_missing_values()` from the first version (#68) still compile, so older code did not break.

**Any number type in and out (James, 5 November).** `numeric.hpp` and `typed_column.hpp` let callers use any number type at the edges, while a `Column` still stores `double`. A `Column` can be built from `std::vector<int>` or any other numeric type, `df["size"].as<int>()` and `read_csv_column<int>(path, "size")` read a column back as that type, and `fill_missing`, `parse_number` and `filter_numeric` accept any numeric type. A value that would change (a fraction into an `int`, a number out of range, a missing value into an integer) throws `std::invalid_argument` naming the column and row, so nothing is rounded silently. `NumericColumn<T>` is a class template holding one column as `T`. The constraint is the shared `Numeric` concept. Full details: `docs/templets.md`.

## 4. How it is tested

- **Normal use:** `iris.csv` and `tips.csv` load with the right number of rows and columns.
- **Messy files** (`data/input/messy.csv`): commas and quotes inside quoted cells, empty cells, `NA`, and junk text in a numeric column.
- **Edge cases:** a byte order mark, `\r\n` endings, blank lines, spaces around cells, line breaks inside quotes.
- **Errors:** an empty file, a duplicate header, an unterminated quote, a row with too many cells, an unknown column, and a row index out of range all throw the right exception. With `BadRowPolicy::Skip`, a bad row is skipped instead.
- **Cleaning:** each function is checked on a small table, and the input table is checked to be unchanged. `drop_nan` is checked to keep the order and to give the same numbers as `drop_missing`.

## 5. Limitations

- CSV only. No JSON or Excel, and dates stay as text.
- The whole file is read into memory.
- A quoted `"NA"` stays as the text `NA`. Only unquoted NA words are missing.
- Text columns have no category type.

## 6. Who changed it

- **James:**
  - first version: `Column`, `DataFrame`, the CSV reader and the sample files (#68, merged 25 October)
  - the full module: the state machine, `CsvOptions`, the cleaning functions, `docs/data.md` and the 49 tests (#105, 1 November)
- **Oscar:** rebuilt #105 on the current `main` as #108, keeping James's commits, and added `preprocessing.cpp` and the tests to CMake.
- **End-of-project fixes, done with Claude Code:** `data_examples` now shows filtering, column selection and missing values (#131).
- **End-of-project fix, done with Claude Code (5 November):** added `drop_nan`, used it in the ECDF, joint plot and statistics instead of three copies of the same code, and made the examples use `drop_missing` and `df["name"]` (`docs/bug-report.md`, third check).
- **James (5 November):** templates for any numeric type: `numeric.hpp`, `typed_column.hpp`, `Column::as<T>`, `read_csv_column<T>`, the template versions of `fill_missing`, `parse_number` and `filter_numeric`, 23 tests and `docs/templets.md` (#152). Member 4 merged it onto the current `main`, made it use the shared `Numeric` concept, and registered the tests.
- **This guide** was written by Member 4 with Claude Code, from James's `docs/data.md` and his code.
