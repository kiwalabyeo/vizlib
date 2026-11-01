# WEEK 4 PROGRESS REPORT

KIZITO MARK JAMES

25/U/30991/PS

BCCE

> Square brackets [like this] are things I could not know. Check each one against your Git history, then correct or delete it before you submit.
> Save this file as `reports/week-04/contributions/Mark.md`.

## 1. Introduction

During Week 4, I worked on invalid-input handling and edge cases in the data loading and preprocessing module, and on supporting the other members as they connected their plots and statistics to my `DataFrame`. The aim was that every kind of bad input ends in a clear error message, and never in a crash or in silently wrong numbers. This work builds on the reader and the cleaning functions from Week 3. [State here whether the data module was merged this week.]

This report explains the work completed during Week 4, how I applied object-oriented programming concepts in my code, the challenges I faced, and how I used artificial intelligence during development.

## 2. Work Completed During Week 4

### 2.1 Invalid-input handling

I followed the one rule that the group agreed on: `std::runtime_error` for things like a missing file, and `std::invalid_argument` for bad input. Every case below has a test that checks for the exact exception:

- a file that cannot be opened (`std::runtime_error`);
- empty input, and input that has only blank lines;
- duplicate column names in the header;
- an unterminated quote, and extra text after a closing quote;
- a row with the wrong number of fields;
- an unknown column name;
- asking for numbers from a text column, or for text from a numeric column;
- filling a numeric column with text, or a text column with a number;
- taking the mean or median of a column that has no values.

The error messages say where the problem is. For a row with the wrong number of fields, the reader reports the line:

```cpp
if (rec.fields.size() != n_cols) {
    if (options.bad_rows == BadRowPolicy::Skip) continue;
    fail(rec.line, "expected " + std::to_string(n_cols) + " fields but found " +
                       std::to_string(rec.fields.size()));
}
```

A quoted cell can contain a line break, so the reader counts real line breaks. The line number in the message points to where the bad row starts. A message looks like this: `CSV error on line 3: expected 3 fields but found 2`.

### 2.2 Options for the user

I added a `CsvOptions` structure, so that a user can choose how forgiving the reader is:

```cpp
struct CsvOptions {
    char delimiter = ',';
    bool has_header = true;
    BadRowPolicy bad_rows = BadRowPolicy::Throw;
    std::vector<std::string> na_values = {"NA", "N/A", "NaN", "nan", "null", "NULL"};
};
```

The user can choose another delimiter such as `;`, say that the file has no header row (the columns are then called `col0`, `col1` and so on), give their own list of words that mean "missing", and choose between throwing on a bad row and skipping it.

### 2.3 One rule for missing values

Every module of the library must treat a missing number in the same way. In my code a numeric cell is missing when it is NaN, and a text cell is missing when its flag is set:

```cpp
bool Column::is_missing(std::size_t i) const {
    if (i >= size()) throw std::out_of_range("Column '" + name_ + "': row index out of range");
    return is_numeric() ? std::isnan(numbers_[i]) : missing_[i] != 0;
}
```

The advantage is that a column can be passed directly to a function that takes `std::vector<double>`. The risk is that a function which does not skip NaN returns NaN, so the other members must call `drop_missing()` first or skip NaN themselves. I told the group about this. [Confirm. My Week 2 research file described `std::optional<double>`. If the code you submit uses NaN, this is a change from the plan, and it must have gone through a pull request with the affected members as reviewers.]

If a column has a few bad cells, for example `n/a` in an `age` column, the reader makes the whole column text instead of guessing. `to_numeric(df, "age", true)` then turns the bad cells into missing values when the user wants that.

### 2.4 Documentation, example and sample file

I wrote `docs/data.md`, which explains every public function with the errors that it throws and the limits of the module. Every public function in the three headers has a comment that says what it takes, returns and throws. I also wrote `examples/data_example.cpp`, which loads a messy file, converts and fills a column, drops the rows with no score, and loads `tips.csv` into a vector that is ready for the ECDF plot. I added `data/input/messy.csv`, a small file with a name that contains a comma, a name with quotes inside it, empty cells, `NA`, and text in a numeric column. [Confirm.]

### 2.5 Support for other members

[Helped Members 2, 3 and 4 to read their columns from `df["column"]` and tested with their real inputs: write the names and what you changed.] [Reviewed pull requests from: names and PR numbers.]

## 3. Application of Object-Oriented Programming Concepts

### 3.1 Encapsulation

The `DataFrame` class keeps its list of columns private and controls what goes into it. In `src/dataframe.cpp`, the `add_column()` function checks the name and the length before it stores a column:

```cpp
void DataFrame::add_column(Column column) {
    if (index_of(column.name()) != npos)
        throw std::invalid_argument("DataFrame already has a column named '" + column.name() + "'");
    if (!columns_.empty() && column.size() != n_rows())
        throw std::invalid_argument("Column '" + column.name() + "' has the wrong number of rows");
    columns_.push_back(std::move(column));
}
```

Because `columns_` is private, there is no other way to add a column, so a `DataFrame` can never have two columns with the same name or columns of different lengths. A test checks that a failed `add_column()` leaves the frame unchanged. I also used a private helper function, `index_of()`, that other classes cannot call.

### 3.2 Polymorphism

I applied compile-time polymorphism through function overloading. The function `read_csv()` has three versions that read from different sources:

```cpp
DataFrame read_csv(const std::string& path, const CsvOptions& options = {});
DataFrame read_csv(std::istream& in, const CsvOptions& options = {});
DataFrame parse_csv(const std::string& text, const CsvOptions& options = {});
```

The first version opens a file and calls the second, and the second reads from any stream. The overloads share the same code, and this made the tests easy to write because they can use text in memory.

I also let the caller choose the behaviour of a function at run time with `std::function`. For example, `filter_numeric(df, "score", [](double s) { return s >= 30; })` keeps the rows for which the function returns true, and the same function can be used with any rule.

### 3.3 Inheritance

I did not use inheritance in the classes developed for my part of the project during the period covered by this report. `Column` and `DataFrame` were developed without deriving them from other classes, and `CsvOptions` is a simple structure that holds settings.

## 4. Challenges Faced

[If the pull request was late, say why in your own words: waiting for reviews, CI failing on Windows, or a redesign.]

The main challenge was agreeing one meaning of "missing" for the whole library. If one function skips a missing value and the next function turns it into a wrong result, the plot is wrong without any error. Writing the rule down once, and giving `drop_missing()` as the standard way to deal with it, took more effort than the code.

Another challenge was error messages. When something goes wrong deep inside a plot, the user should see which file and which line caused it, so the messages from my module name the line, the column or the value.

A third challenge was the size of the work. The module is large, so I planned to split it into several small pull requests (headers, `DataFrame`, the reader, the cleaning functions), each with its own tests, so that reviewers could read them within 24 hours.

## 5. Use of AI
- Tool: Gemini / AI Assistant
- What for: Assisting with standard C++17 std::optional interface design and drafting research markdown structure.
- Why: To streamline C++ header design according to group project requirements and fast-track repository contribution.


## 6. Conclusion and Next Steps

During Week 4, I completed the invalid-input handling of the data module, added `CsvOptions`, fixed one rule for missing values, and wrote the documentation, the example and the `messy.csv` sample file. My code shows encapsulation in `DataFrame::add_column()` and compile-time polymorphism through the overloaded `read_csv()` and `fill_missing()` functions. I did not use inheritance.

My next steps are to finish anything still open and then only fix bugs after the code freeze on Saturday 31 October, to write my own paragraph for the README Contributions section, to make sure `read_csv()` works inside `full_pipeline.cpp` together with every other module on Linux, Windows and macOS, and to review at least one pull request from another member.