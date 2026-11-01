# WEEK 5 PROGRESS REPORT

KIZITO MARK JAMES

25/U/30991/PS

BCCE

> Square brackets [like this] are things I could not know. Check each one against your Git history, then correct or delete it before you submit.
> Save this file as `reports/week-05/contributions/Mark.md`.

## 1. Introduction

Week 5 was the finishing week, and the code freeze was on Saturday 31 October. The data loading and preprocessing module is complete: it reads a CSV file into a `DataFrame`, reports bad input with clear errors, and provides the cleaning functions that the statistics and plot modules rely on. After the freeze, only bug fixes are allowed. [PR #__ merged on __ October. Confirm.]

This report explains the final state of the module, how I applied object-oriented programming concepts in my code, the challenges I faced, and how I used artificial intelligence during development.

## 2. Work Completed During Week 5

### 2.1 Final files of the module

| Part | Files |
|---|---|
| Data classes | `include/vizlib/dataframe.hpp`, `src/dataframe.cpp` |
| CSV reader | `include/vizlib/csv_reader.hpp`, `src/csv_reader.cpp` |
| Cleaning functions | `include/vizlib/preprocessing.hpp`, `src/preprocessing.cpp` |
| Tests | `tests/test_dataframe.cpp`, `tests/test_csv_reader.cpp`, `tests/test_preprocessing.cpp`, `tests/data_path.hpp` |
| Sample data | `data/input/iris.csv`, `tips.csv`, `messy.csv` |
| Documentation and example | `docs/data.md`, `examples/data_example.cpp` |

### 2.2 What the module does

A `Column` is numeric (stored as `double`) or text (stored as strings with a missing flag for each cell). A `DataFrame` keeps its columns in order, refuses duplicate names and columns of different lengths, and finds a column by name (`df["tip"]`) or by position. A `Column` converts to `std::vector<double>`, so `ecdf_plot(ax, df["total_bill"])` works as in the project plan:

```cpp
operator const std::vector<double>&() const { return values(); }
```

The function `read_csv()` reads from a file path, a stream or a string. It handles quoted cells with commas, doubled quotes and line breaks inside quotes, Windows and Linux line endings, a byte order mark, blank lines, spaces around cells, and empty or `NA` cells. It decides by itself which columns are numeric.

The cleaning functions are `drop_missing`, `fill_missing`, `fill_missing_mean`, `fill_missing_median`, `to_numeric`, `filter_rows`, `filter_numeric`, `select_columns` and `drop_columns`. None of them changes its input.

### 2.3 Names from the first version

Other members may have written code against the first version of my module. So I kept the old names working. `row_count()`, `column_count()`, `get_column()`, `add_column(name, data)` and `handle_missing_values()` still exist. For example, the old function name now calls the new one:

```cpp
DataFrame handle_missing_values(const DataFrame& df) {
    return drop_missing(df);
}
```

Two behaviours changed: an unknown column now throws `std::invalid_argument` and not `std::out_of_range`, and adding a column with an existing name throws and does not overwrite the old column. This is described in `docs/data.md`.

### 2.4 Testing and quality

The module has 49 test cases with 199 assertions, and all of them pass. They cover normal use, every error path, and the sample files: `iris.csv` loads as 150 rows and 5 columns, `tips.csv` as 244 rows and 7 columns (even though every value is in quotes), and `messy.csv` keeps its problems. The code compiles with `-Wall -Wextra -Wpedantic` without warnings, and it runs without errors under AddressSanitizer and UndefinedBehaviorSanitizer. The tests find the sample files from any build folder, because of the small helper in `tests/data_path.hpp`. [Confirm that CI is green on Linux, Windows and macOS.]

### 2.5 Documentation and integration

`docs/data.md` is complete: it has the errors table, every public function, the CSV rules, the limits and the tests. Every public function in the headers has a comment. [Checked that `read_csv()` works in `full_pipeline.cpp` with all the other modules: write what you tested and any problem you found.] [Reviewed pull requests from: names and PR numbers.] [Wrote my paragraph for the README Contributions section.]

## 3. Application of Object-Oriented Programming Concepts

### 3.1 Encapsulation

I applied encapsulation in `Column` and `DataFrame`. The data of both classes is private, and the public functions check every change. For example, `Column::values()` returns the numbers only for a numeric column and throws `std::invalid_argument` for a text column:

```cpp
const std::vector<double>& Column::values() const {
    if (!is_numeric())
        throw std::invalid_argument("Column '" + name_ +
                                    "' is a text column; convert it with to_numeric() first");
    return numbers_;
}
```

The cleaning functions go one step further and never change their input: they return a new `DataFrame`. This means that a function in another module can read a frame without any risk that my code changes it.

### 3.2 Polymorphism

My code uses compile-time polymorphism through function overloading in several places: `fill_missing()` (numbers and text), `drop_missing()` (all columns or chosen columns), `read_csv()` (path or stream), the two `Column` constructors (numbers or strings), and `DataFrame::add_column()` (a `Column`, or a name with a list of `CellValue`). I also used `std::function` so that the caller chooses the rule in `filter_rows()` and `filter_numeric()`.

```cpp
DataFrame filter_rows(const DataFrame& df,
                      const std::function<bool(const DataFrame&, std::size_t)>& predicate);
```

### 3.3 Inheritance

I did not use inheritance in the classes developed for my part of the project. `Column` and `DataFrame` are related by composition: a `DataFrame` contains a list of `Column` objects. Inheritance in this project is in the plot classes of the other members, for example the planned `Plot` base class. My module does not need a base class, because it has no group of different objects that must be used through one interface.

## 4. Challenges Faced

[The data module was meant to be finished in Week 2, and some parts arrived late, so other members had to test with hand-made vectors for a while. Write the real reason in your own words.]

The limits that remain are written in `docs/data.md`: the module reads CSV only (JSON was dropped in the project plan), the whole file is read into memory, dates stay as text, and a quoted `"NA"` stays as the text `NA`, because only unquoted NA words are missing.

Other modules must skip NaN or call `drop_missing()` before they compute statistics. This was told to the group in Week 4. [Confirm.]

The group rules say that every member is questioned on their own code, and that nobody may commit code that they cannot explain. I spent time reading the whole module again, including the quote-handling loop in the reader and the number check, so that I can explain it in the presentation.

## 5. Use of AI
- Tool: Gemini / AI Assistant
- What for: Assisting with standard C++17 std::optional interface design and drafting research markdown structure.
- Why: To streamline C++ header design according to group project requirements and fast-track repository contribution.

## 6. Conclusion and Next Steps

During Week 5, I finished and froze the data loading and preprocessing module: the `Column` and `DataFrame` classes, the CSV reader and the cleaning functions, with 49 passing test cases, documentation and an example. My code shows encapsulation in `Column` and `DataFrame` and compile-time polymorphism through function overloading. I did not use inheritance.

My next steps for 1 to 5 November are a fresh-machine test (clone the repository on a computer that has never built it and follow only the README), fixing any bug in my module, helping with the final report and the full AI-use declaration, and preparing for the slides and the demonstration, including questions on the data module and on the whole pipeline.