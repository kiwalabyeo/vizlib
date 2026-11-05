# Templates and numeric types in the data module

> **Note (5 November):** Member 4 merged this onto the current `main` (#152 was based on an old version of `main`). The data module now uses the project's shared `Numeric` concept from `concepts.hpp` instead of defining its own, and `tests/test_numeric_types.cpp` is registered in CMake.

Lecturer's request: matrices, models and data classes must accept all numeric
types (int, double, float, short, ...), using appropriate concepts. This page
documents what the data module now does about it.

## 1. The idea

A `Column` stores its numbers as `double`, so the whole library has one number
type to work with. The templates let the **caller** use any numeric type at the
edges: when putting numbers in, and when taking numbers out. Nothing is rounded
or wrapped silently. If a value does not fit the requested type, the code throws
`std::invalid_argument` that names the column and row.

## 2. What counts as "numeric": `numeric.hpp`

| Name | Kind | Meaning |
|---|---|---|
| `is_numeric_type_v<T>` | variable template | true for every arithmetic type except `bool` |
| `Numeric` | concept (C++20) | the project's shared concept from `concepts.hpp`: any integer or floating-point type except `bool` and `char` |
| `VIZLIB_NUMERIC_TEMPLATE(T)` | macro | short for `template <Numeric T>` |
| `is_representable<T>(double x)` | function template | can `x` be stored in a `T` without changing it? |
| `to_double_exact<T>(T x)` | function template | convert to `double`, throw if the double would not hold it exactly |

**One concept for the whole library.** The project is built as C++20, and the
data module uses the same `vizlib::Numeric` concept as the plots and the
statistics (`concepts.hpp`), so every module accepts exactly the same types.
`VIZLIB_NUMERIC_TEMPLATE(T)` is kept as a short name for `template <Numeric T>`.
Compiling with a type that is not numeric, such as `std::string`, is a compile
error.

## 3. The templates

| Template | File | Use |
|---|---|---|
| `Column(name, const std::vector<T>&)` | dataframe.hpp | build a column from `vector<int>`, `vector<float>`, ... |
| `Column::as<T>()` | dataframe.hpp | read a column as `vector<T>` |
| `DataFrame::column_as<T>(name)` | dataframe.hpp | same, by column name |
| `fill_missing(df, col, T value)` | preprocessing.hpp | fill missing cells with any numeric value |
| `parse_number(string, T& out)` | preprocessing.hpp | parse text into any numeric type |
| `filter_numeric<T>(df, col, predicate)` | preprocessing.hpp | predicate takes `T`, e.g. `int` |
| `read_csv_column<T>(path, col)` | csv_reader.hpp | read one CSV column as `vector<T>` |
| `NumericColumn<T>` | typed_column.hpp | **class template**: a column held as `T` |

The original non-template functions (`double` versions) are unchanged. When the
argument is a `double` the compiler still picks the non-template one, so existing
code keeps working.

```cpp
DataFrame tips = read_csv("data/input/tips.csv");

std::vector<int>   size = tips["size"].as<int>();               // whole numbers
std::vector<float> bill = tips.column_as<float>("total_bill");
std::vector<int>   same = read_csv_column<int>("data/input/tips.csv", "size");

DataFrame big = filter_numeric<int>(tips, "size", [](int s) { return s >= 4; });
DataFrame fixed = fill_missing(df, "age", 30);                    // int value
NumericColumn<int> sizes(tips["size"]);                           // class template
```

## 4. Conversion rules (`is_representable<T>`)

| Target type | Accepted | Rejected (throws) |
|---|---|---|
| integer (`int`, `short`, `unsigned`, `long long`, ...) | whole numbers inside the type's range | NaN (missing), `2.5`, out of range, negative into unsigned |
| `float` | any value inside float's range, NaN, infinity | finite values too large for float |
| `double`, `long double` | everything | nothing |

For integer ranges the check compares with `2^digits` (exactly representable as
a double), so it is exact for every type including 64-bit.

Putting integers **into** a `Column` throws if a double cannot hold the value
exactly (a 64-bit integer above 2^53).

## 5. UML

- `docs/uml/data_module_classes.png`: classes, templates, enums and the
  relations between them (source: `data_module_classes.mmd`).
- `docs/uml/data_module_flow.png`: how a CSV file becomes typed vectors
  (source: `data_module_flow.mmd`).

Relations drawn: `DataFrame` **composes** `Column`s (a DataFrame has columns, no
inheritance); `NumericColumn<T>` depends on `Column`; `CsvOptions` has a
`BadRowPolicy`; the reader and preprocessing functions depend on `DataFrame` and
return new ones; every template is constrained by `Numeric`.

## 6. Tests (`tests/test_numeric_types.cpp`)

23 test cases: the trait and concept, `is_representable` for integer and
floating types, constructing columns from many types, `as<T>` with NaN,
fractions, range and text errors, `fill_missing` with int/long/unsigned/float/
double/string, `parse_number` into several types, `filter_numeric<T>`,
`read_csv_column<T>` on `tips.csv`, and `NumericColumn<T>`. Together with the
earlier data tests: 74 test cases, all passing, with no warnings under
`-Wall -Wextra -Wpedantic`.

## 7. Limits

- Values are stored as `double` inside a `Column`, so the exact integer range is
  up to 2^53. Larger 64-bit values are rejected rather than rounded.
- `Column`/`DataFrame` themselves are not class templates; the templates are at
  the input/output edges. Making `Column<T>` generic would change the interface
  every other member uses.
