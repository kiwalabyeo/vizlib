# Data: reading CSV files and cleaning them

- **Owner:** Kizito Mark James (Member 1)
- **Headers:** `include/vizlib/dataframe.hpp`, `csv_reader.hpp`, `preprocessing.hpp`
- **Sources:** `src/dataframe.cpp`, `src/csv_reader.cpp`, `src/preprocessing.cpp`
- **Tests:** `tests/tests_dataframe.cpp`, `test_csv_reader.cpp` and `test_preprocessor.cpp` (49 test cases, 199 assertions)
- **Example:** `data_examples`, and `pipeline`, which reads `data/input/iris.csv`
- **User guide:** `docs/data.md` (every function and every error)

## 1. What it does

It is the first step of the pipeline. It reads a CSV file into a `DataFrame` (a table of named columns), and cleans it. Every other module takes its numbers from here.

```cpp
#include <vizlib/vizlib.hpp>

auto df = vizlib::read_csv("data/input/tips.csv");
df = vizlib::drop_missing(df);
const std::vector<double>& bill = df["total_bill"];   // ready for ecdf_plot or summaryStatistics
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

## 3. Why it is written this way

- **Classes and encapsulation.** `Column` and `DataFrame` keep their data private. `add_column` throws if the name is already used or the length is wrong, so a `DataFrame` always has columns of the same length.
- **`enum class`.** `Column::Type` (`Numeric` or `Text`), `BadRowPolicy` (`Throw` or `Skip`) and the reader's `State` are `enum class`, so they cannot be mixed up with plain numbers.
- **A conversion operator.** A `Column` turns into `const std::vector<double>&` by itself. That is why `ecdf_plot(ax, df["tip"])` works with no extra step. It returns a reference, so nothing is copied.
- **`const` and references.** The cleaning functions take `const DataFrame&` and return a new one, the same as pandas.
- **Exceptions with line numbers.** A bad file throws `std::invalid_argument` with the line number in the message. A file that cannot be opened throws `std::runtime_error`.
- **Options in a struct.** `CsvOptions` holds the delimiter, the header flag, the bad-row policy and the missing-value words, with defaults. So `read_csv(path)` works with no options.
- **No templates.** CSV cells are always text or `double`, so a template would add nothing.
- **Old names still work.** `row_count()`, `get_column()` and `handle_missing_values()` from the first version (#68) still compile, so older code did not break.

## 4. How it is tested

- **Normal use:** `iris.csv` and `tips.csv` load with the right number of rows and columns.
- **Messy files** (`data/input/messy.csv`): commas and quotes inside quoted cells, empty cells, `NA`, and junk text in a numeric column.
- **Edge cases:** a byte order mark, `\r\n` endings, blank lines, spaces around cells, line breaks inside quotes.
- **Errors:** an empty file, a duplicate header, an unterminated quote, a row with too many cells, an unknown column, and a row index out of range all throw the right exception. With `BadRowPolicy::Skip`, a bad row is skipped instead.
- **Cleaning:** each function is checked on a small table, and the input table is checked to be unchanged.

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
- **This guide** was written by Member 4 with Claude Code, from James's `docs/data.md` and his code.
