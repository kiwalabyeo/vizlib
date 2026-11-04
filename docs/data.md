# Data loading and preprocessing (`data`)

Owner: Member 1 (Mark)
Headers: `dataframe.hpp`, `csv_reader.hpp`, `preprocessing.hpp`. Sources: `src/dataframe.cpp`, `src/csv_reader.cpp`, `src/preprocessing.cpp`.

This module turns a CSV file into a `DataFrame` and cleans it. Everything after it in the pipeline (stats, ECDF, rug, joint plot) takes the numbers from here.

```cpp
#include <vizlib/vizlib.hpp>
auto df = vizlib::read_csv("data/input/tips.csv");
df = vizlib::drop_missing(df);
const std::vector<double>& bill = df["total_bill"];   // ready for ecdf_plot / summary
```

## Errors

| Situation | Exception |
|---|---|
| File cannot be opened | `std::runtime_error` |
| Bad content: empty input, duplicate header, unterminated quote, uneven row, bad quote | `std::invalid_argument` (message names the line) |
| Unknown column, text column used where numbers are needed, length mismatch | `std::invalid_argument` |
| Row index out of range | `std::out_of_range` |

## Column

A column is **numeric** (`double`, `NaN` = missing) or **text** (`std::string` plus a missing flag per cell).

| Function | What it does |
|---|---|
| `name()`, `size()`, `is_numeric()` | basics |
| `is_missing(i)`, `count_missing()` | missing-cell checks |
| `values()` | the numbers as `const std::vector<double>&`; throws on a text column |
| `text()` | the strings of a text column; throws on a numeric column |
| `as_string(i)` | any cell as text (missing gives `""`) |
| `take(rows)` | new column with only those rows |
| implicit conversion | a `Column` converts to `const std::vector<double>&`, so `ecdf_plot(ax, df["tip"])` works |

## DataFrame

| Function | What it does |
|---|---|
| `add_column(Column)` | append; throws on duplicate name or wrong length |
| `n_rows()`, `n_cols()`, `empty()` | size |
| `has_column(name)`, `column_names()` | lookup |
| `df["name"]`, `column(name)`, `column(index)` | get a column |
| `replace_column(Column)` | swap a column for a new one of the same length |
| `take_rows(rows)` | new frame with only those rows |
| `to_string(max_rows)` | printable preview |

## read_csv

```cpp
DataFrame read_csv(const std::string& path, const CsvOptions& options = {});
DataFrame read_csv(std::istream& in, const CsvOptions& options = {});
DataFrame parse_csv(const std::string& text, const CsvOptions& options = {});
```

`CsvOptions`: `delimiter` (default `,`), `has_header` (default true), `bad_rows` (`Throw` default, or `Skip`), `na_values` (default `NA N/A NaN nan null NULL`).

Handled: quoted fields containing commas, doubled quotes (`""`), line breaks inside quotes, CRLF and LF line endings, a UTF-8 byte order mark, blank lines, spaces around cells, empty cells (missing), rows with the wrong number of fields (error with line number, or skipped).

A column is numeric when every non-missing cell is a plain decimal number (`12`, `-3.5`, `2e3`). `inf`, `nan`, hex such as `0x1A`, and numbers too large for a double are **not** numbers. One bad cell makes the whole column text; use `to_numeric(df, col, true)` to turn the bad cells into missing values instead.

## Preprocessing

All functions return a new `DataFrame`; the input is never changed.

| Function | What it does |
|---|---|
| `drop_missing(df)` | remove rows with a missing cell in any column |
| `drop_missing(df, {"a","b"})` | same, looking only at those columns |
| `fill_missing(df, col, 0.0)` / `fill_missing(df, col, std::string("n/a"))` | fill a numeric / text column with a constant |
| `fill_missing_mean(df, col)`, `fill_missing_median(df, col)` | fill a numeric column with its mean / median |
| `to_numeric(df, col, coerce=false)` | convert a text column; bad cells throw, or become missing if `coerce` |
| `filter_rows(df, pred)`, `filter_numeric(df, col, pred)` | keep rows where the predicate is true |
| `select_columns(df, names)`, `drop_columns(df, names)` | keep / remove columns |
| `parse_number(str, out)` | the number test the reader uses |

## Limits

- CSV only. No JSON, no dates (dates stay text).
- The whole file is read into memory.
- Text cells that are quoted `"NA"` are kept as the text `NA`; only unquoted NA words are missing.
- Text columns have no automatic category type.

## Tests

`tests/tests_dataframe.cpp`, `tests/test_csv_reader.cpp`, `tests/test_preprocessor.cpp` (49 test cases). Sample data: `data/input/iris.csv`, `tips.csv`, and `messy.csv` (quoted commas, a quoted quote, empty cells, `NA`, junk in a numeric column).

## Names from the first version of the library

Code written against the first version still compiles: `row_count()`, `column_count()`, `get_column(name)` (returns a copy as `std::vector<CellValue>`, where `CellValue` is `std::variant<double, std::string>`), `add_column(name, vector<CellValue>)` and `handle_missing_values(df)` (same as `drop_missing(df)`). Differences from the first version: an unknown column throws `std::invalid_argument` (not `std::out_of_range`), and adding a column with an existing name throws instead of overwriting it. New code should use `Column`, `df["name"]` and the functions above.
