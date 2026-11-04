# WEEK 3 PROGRESS REPORT

KIZITO MARK JAMES

25/U/30991/PS

BCCE

> **Note on this report.** On 4 November Member 4 (Oscar), with Claude Code, replaced the placeholders left in this report (such as "[Confirm.]") with the facts from GitHub: pull request numbers, dates and reviews. Where something could not be checked, it was removed. The dates show when the work reached GitHub.

## 1. Introduction

During Week 3, I worked on proving that the data loading and preprocessing part of the data visualisation library works on real files. My module is the first step of the pipeline, because every statistic and every plot takes its numbers from the `DataFrame` and `Column` classes that I built in Week 2. The Week 3 goal for the group was a CSV file going in and an ECDF plot with axes coming out as a PNG, so the data module had to be reliable first. On GitHub, the first version of the module (#68: the reader, `DataFrame`, the cleaning functions and the sample files) was opened on 24 October and merged on 25 October, after this week ended. The tests, the quote-handling reader and `parse_number()` described below reached GitHub on 1 November, in #105, which was rebuilt as #108.

This report explains the work completed during Week 3, how I applied object-oriented programming concepts in my code, the challenges I faced, and how I used artificial intelligence during development.

## 2. Work Completed During Week 3

### 2.1 Unit tests for the whole module

I wrote the unit tests for all three parts of my module, with one test file for each, as the project plan requires:

- `tests/test_dataframe.cpp` tests the `Column` and `DataFrame` classes.
- `tests/test_csv_reader.cpp` tests `read_csv()` on normal files and on bad ones.
- `tests/test_preprocessing.cpp` tests `drop_missing()` and the other cleaning functions.

The tests use doctest, and each test checks one behaviour. For example, this test checks that a missing file gives the exception that the group agreed on:

```cpp
TEST_CASE("read_csv: missing file throws runtime_error") {
    CHECK_THROWS_AS(read_csv("definitely/not/here.csv"), std::runtime_error);
}
```

The other edge cases that I tested are an empty file, a file with only a header row, empty cells, the words `NA`, `N/A`, `NaN` and `null`, rows with more or fewer fields than the header, duplicate column names, blank lines, spaces around cells, Windows (CRLF) line endings, and a byte order mark at the start of a file.

### 2.2 A CSV reader that works on real files

My Week 2 reader split every line on commas with `std::getline`. The tests showed that this is not enough: `tips.csv` has every header and text value inside double quotes, and a real file can have a comma inside a quoted cell such as `"Smith, John"`. I changed the reader so that it goes through the whole file one character at a time and keeps track of whether it is inside quotes. It has four states, and the part that handles a quoted cell looks like this:

```cpp
enum class State { FieldStart, Unquoted, Quoted, AfterQuote } state = State::FieldStart;

case State::Quoted:
    if (c == '"') {
        if (i + 1 < text.size() && text[i + 1] == '"') {
            field.text.push_back('"');   // "" inside quotes means one quote
            ++i;
        } else {
            state = State::AfterQuote;
        }
    } else {
        if (c == '\n') ++line;
        field.text.push_back(c);
    }
    break;
```

With this the reader handles commas inside quotes, doubled quotes, and line breaks inside quotes. A quoted number such as `"16.99"` is still recognised as a number.

### 2.3 A stricter check for numbers

`std::stod` accepts text such as `inf`, `nan` and `0x1A`, and it also accepts a number with extra text after it. That would turn a column of codes into numbers by accident. I wrote a small function, `parse_number()`, that accepts only plain decimal numbers such as `12`, `-3.5` and `2e3`. It also forces the plain "C" locale, so that `3.5` is read in the same way on Linux, Windows and macOS.

```cpp
if (t.find_first_not_of("0123456789+-.eE") != std::string::npos) return false;
if (t.find_first_of("0123456789") == std::string::npos) return false;

std::istringstream in(t);
in.imbue(std::locale::classic());   // always '.' as the decimal point
```

### 2.4 The rest of the cleaning functions

Next to `drop_missing()`, I added `fill_missing()`, `fill_missing_mean()`, `fill_missing_median()`, `to_numeric()`, `filter_rows()`, `filter_numeric()`, `select_columns()` and `drop_columns()`. Every function returns a new `DataFrame` and never changes its input, so the functions can be chained. For example, `drop_missing()` works out which rows to keep and then builds a new frame from them:

```cpp
std::vector<std::size_t> keep;
for (std::size_t r = 0; r < df.n_rows(); ++r) {
    bool ok = true;
    for (const Column* c : check)
        if (c->is_missing(r)) { ok = false; break; }
    if (ok) keep.push_back(r);
}
return df.take_rows(keep);
```

### 2.5 Sample data

I added `iris.csv` (150 rows) next to `tips.csv` (244 rows) in `data/input/`, so that the plots can be tried on two different datasets. Both files came in #68.

## 3. Application of Object-Oriented Programming Concepts

### 3.1 Encapsulation

I applied encapsulation in the `Column` and `DataFrame` classes by keeping the data private and giving access only through public member functions. In `include/vizlib/dataframe.hpp`, the `Column` class keeps its name, type, numbers, strings and missing flags private:

```cpp
private:
    std::string name_;
    Type type_;
    std::vector<double> numbers_;
    std::vector<std::string> strings_;
    std::vector<char> missing_;  // text columns only
```

Other members of the group cannot change these directly. They use public functions such as `name()`, `size()`, `is_missing(i)`, `values()` and `text()`. This protects the data. For example, `values()` throws `std::invalid_argument` if it is called on a text column, so a plot function can never receive text by mistake. In the same way, `DataFrame` keeps its columns private and checks every new column before it stores it.

### 3.2 Polymorphism

I applied compile-time polymorphism through function overloading. The functions have the same name, and the compiler selects the right one from the arguments. In `include/vizlib/preprocessing.hpp`, `fill_missing()` has one version for numbers and one for text:

```cpp
DataFrame fill_missing(const DataFrame& df, const std::string& column, double value);
DataFrame fill_missing(const DataFrame& df, const std::string& column, const std::string& value);
```

`fill_missing(df, "age", 0.0)` fills a numeric column, and `fill_missing(df, "group", std::string("unknown"))` fills a text column. `drop_missing()` is overloaded in the same way: one version looks at all columns, and one looks only at the columns in a list. The two `Column` constructors are also overloaded, one for a vector of numbers and one for a vector of strings.

### 3.3 Inheritance

I did not use inheritance in the classes developed for my part of the project. `Column` and `DataFrame` are separate classes that are related by composition, because a `DataFrame` contains a list of `Column` objects.

## 4. Challenges Faced

The first challenge was that the data module blocks the other members. Members 2, 3 and 4 take their numbers from a `DataFrame`, so until my tests passed and my pull request was merged they could only test with hand-made vectors.

The second challenge was that real files are messier than my research file assumed. A reader that only splits on commas fails on `tips.csv`. Finding this with a test, and not in the final demonstration, is the reason why I wrote the tests early.

The third challenge was deciding what counts as a number. If `0x1A` or `inf` is silently turned into a number, the plots would show wrong data without any error, so I made the check strict.

I also had to learn the pull request workflow. My first pull requests (#42 to #44) only renamed my research file.

## 5. Use of AI

- Tool: Gemini / AI Assistant
- What for: Assisting with standard C++17 std::optional interface design and drafting research markdown structure.
- Why: To streamline C++ header design according to group project requirements and fast-track repository contribution.


## 6. Conclusion and Next Steps

For the data module I wrote the unit tests, rewrote the CSV reader so that it handles quoted cells, added a strict `parse_number()` function, and added the cleaning functions. My code shows encapsulation in `Column` and `DataFrame`, and compile-time polymorphism through the overloaded `fill_missing()` and `drop_missing()` functions. I did not use inheritance.

My next steps are to handle every kind of invalid input and test it, write `docs/data.md` and a comment on every public function, and help Members 2, 3 and 4 connect their code to `df["column"]`. I will also review at least one pull request from another member.