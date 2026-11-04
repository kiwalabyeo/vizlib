# Statistics: summaries, histogram bins and the KDE curve

- **Owner:** Natukunda Melissa (Member 2)
- **Headers:** `include/vizlib/stats.hpp`, `kde.hpp` (`kde.hpp` also uses `concepts.hpp`)
- **Sources:** `src/stats.cpp`, `src/kde.cpp`
- **Tests:** `tests/test_stats.cpp` (14 test cases), `tests/test_kde.cpp` (12) and the bug 1 to 4 cases in `tests/test_edge_cases.cpp` (7). Most run once for each of 7 number types.
- **Example:** `examples/stats_example.cpp`, which saves `data/output/stats_example.png`

## 1. What it does

It turns a column of numbers into the numbers a plot needs. `summaryStatistics` gives the usual summaries and the bin edges for a histogram. `kde()` works out a smooth density curve (a kernel density estimate), which `KdePlot` draws. This module does no drawing and no file reading.

```cpp
#include <vizlib/csv_reader.hpp>
#include <vizlib/kde.hpp>
#include <vizlib/kde_plot.hpp>
#include <vizlib/stats.hpp>

const vizlib::DataFrame df = vizlib::read_csv("data/input/iris.csv");
const std::vector<double>& petal = df["petal_length"];

const vizlib::summaryStatistics stats(petal);
double middle = stats.median();                            // 4.35
std::vector<double> edges = stats.histogram_bin_edges();   // 9 bins, so 10 edges

const vizlib::KdeResult curve = vizlib::kde(petal);        // 200 points, bandwidth chosen for you
const vizlib::KdePlot plot(curve);                         // ready to draw
```

**Where it fits.** CSV file → `DataFrame` → **this module** → plots. A `DataFrame` column goes in as a `std::vector<double>`. Missing cells arrive as NaN and are left out here.

- `auto_bin_edges` (`histogram.cpp`) takes its number of bins from `histogram_bin_edges()`.
- `JointPlot::with_kde` calls `kde()` for the x and y margin panels.
- `KdePlot` draws the `KdeResult` that `kde()` returns. It never works out a density itself.

**The public calls.**

| Call | Gives back | Throws `std::invalid_argument` when |
|---|---|---|
| `summaryStatistics(data)` | the object | data is empty, all NaN, or has infinity |
| `mean()`, `median()`, `range()`, `standard_deviation()`, `interquartile_range()` | `double` | never |
| `quantile(q)` | `double` | `q` is outside 0 to 1 |
| `mode()` | every most-frequent value, ascending | never |
| `count(value)` | how often `value` occurs | never |
| `histogram_bin_edges()` | k bins give k + 1 edges | never |
| `SturgesBinning(data)`, `FreedmanDiaconisBinning(data)` | an object with the same `histogram_bin_edges()` | same data checks |
| `kde(data, grid_points = 200, bandwidth = 0)` | `KdeResult` (`grid`, `density`, `bandwidth`) | bad data, `grid_points` below 2, or a bandwidth that is negative, NaN or infinite |
| `silverman_bandwidth(data)` | `double` | bad data |

## 2. How it works

**Storing the data.** `collect_data` removes NaN, refuses infinity, and throws if nothing is left. It keeps `Data` in the original order and `Sorted_data` in ascending order. The sort is done once, so the median, range and quantiles are look-ups.

**The summaries.**
- **Quantile:** `rank = q × (n − 1)` is a position in the sorted data, and a value between two neighbours is mixed in proportion (the same as NumPy's default). It uses `n − 1` because C++ arrays start at 0.
- **Standard deviation:** `sqrt(sum((x − mean)²) / n)`, the population version.
- **Mode:** count each value, find the highest count, and return every value with it. That is why it returns a vector.

**Histogram bins.** Two rules, one class each:
- **Sturges:** `ceil(1 + log2(n))` bins of equal width.
- **Freedman–Diaconis:** width = `2 × IQR / cbrt(n)`, bins = `ceil(range / width)`. It uses only the middle half of the data, so it copes with skew.

`summaryStatistics` uses Sturges for 31 to 199 values and Freedman–Diaconis otherwise. If the IQR is 0, Freedman–Diaconis falls back to Sturges. If all values are equal, both return one bin of width 1 around the value. The bin count is capped at 1,000, because one far outlier would otherwise ask for millions; the bins are then made wider to still cover the range.

*Example, {1, 2, 2, 3, 4}:* width = 2 × 1 / cbrt(5) = 1.1696, bins = ceil(3 / 1.1696) = 3, edges 1, 2.1696, 3.3392, 4.5088. The last edge is past the largest value, so `auto_bin_edges` keeps only the bin count and spreads the edges evenly from the smallest to the largest value.

**KDE.** The curve at x is

```
f(x) = 1 / (n × h) × sum over all data points xi of K((x − xi) / h)
K(u) = exp(−u² / 2) / sqrt(2π)          (the Gaussian kernel)
```

Each data point adds a small bell curve of width `h` (the bandwidth), and `1 / (n × h)` makes the area under the curve 1. `gaussian_density_at` does this for one x. `kde()` calls it at `grid_points` evenly spaced x values, from 3 bandwidths below the smallest value to 3 above the largest, so the tails fade out.

If the bandwidth is 0, `silverman_bandwidth` chooses it: `h = 0.9 × spread × n^(−1/5)`. The spread is the smaller of the standard deviation and `IQR / 1.34`, which ignores outliers. If all values are equal the spread is set to 1 so that `h` stays above 0.

*Example, {1, 2, 3}:* standard deviation 0.8165, `IQR / 1.34` = 0.746, so `h = 0.9 × 0.746 × 3^(−1/5)` = 0.539. The grid runs from −0.617 to 4.617, and the density at x = 2 is 0.335.

## 3. Why it is written this way

- **Encapsulation.** `Data` and `Sorted_data` are private, and `collect_data` is the only way in. Nobody can unsort the data or add NaN, so every other method can assume at least one real number. The constructor calls it, so a `summaryStatistics` is valid or it throws.
- **`const` methods.** Every summary is `const`, since working out a mean must not change the data. That is why a `const summaryStatistics` works.
- **Inheritance and polymorphism (Strategy pattern).** `binningStrategy` is an abstract base with one pure virtual function, `histogram_bin_edges()`. The two rules override it, and `stats_example.cpp` holds both in a `std::vector<std::unique_ptr<binningStrategy>>`. A new rule is one new subclass and no other change. Without this the choice would be an `if` chain that grows with every rule. The destructor is `virtual` so deleting through a base pointer is safe. `Sorted_data`, `max_bins` and `single_bin()` are `protected`, shared by the rules but hidden from users.
- **A struct for the KDE result.** `KdeResult` is plain data, not a class, because it is only handed to `KdePlot`. Computing here and drawing in `KdePlot` was agreed in Week 1.
- **A template only where needed.** `kde()` accepts any `Numeric` type, converts once with `to_doubles`, and calls `kde_from_doubles`. `summaryStatistics` takes `std::vector<double>`; other types are converted first.
- **NaN left out, infinity refused.** A missing CSV cell is NaN, and `std::sort` cannot order NaN, so keeping it gave wrong medians with no error. Infinity gives no mean, spread or bin, so it throws.
- **Exceptions.** Bad input throws `std::invalid_argument` with a clear message. The bandwidth check is `!(h > 0)`, because `h <= 0` is false for NaN.

## 4. How it is tested

- **Summaries and bins** (`test_stats.cpp`): for {1, 2, 2, 3, 4} the mean is 2.4, the median 2, the standard deviation 1.0198, Q1 2, Q3 3, the IQR 1 and the mode {2}. Freedman–Diaconis gives edges 1, 2.16961, 3.33921, 4.50882 and Sturges gives 1, 1.75, 2.5, 3.25, 4. Each test runs for `short`, `int`, `long`, `long long`, `unsigned int`, `float` and `double`.
- **KDE** (`test_kde.cpp`): the area under the curve is 1; the grid has the asked size and runs 3 bandwidths past the data; symmetric data peaks in the middle; `silverman_bandwidth` follows the formula; bad input throws.
- **Edge cases** (`test_edge_cases.cpp`): NaN in any position gives the same results as data without it; all-NaN and infinity throw; one far outlier gives a limited number of bins; an IQR of 0 and all-equal values give valid bins; the KDE leaves out NaN and refuses a NaN or infinite bandwidth.
- **With the plots** (`test_joint_plot.cpp`): `KdePlot` built from `kde()`, and `JointPlot::with_kde`.

## 5. Limitations

- **`mode()` and `count()` compare doubles exactly.** Measured data often has no repeated value, and then `mode()` returns every value.
- **The standard deviation divides by n**, not n − 1, so it is slightly smaller than the sample version for small datasets.
- **Only the Gaussian kernel.** For data with several separate peaks, Silverman's rule can smooth too much; passing a smaller bandwidth helps.
- **`kde()` does n × `grid_points` calculations**, so a very large dataset is slow.
- **Fewer than 31 values use Freedman–Diaconis**, which can give few bins (5 values give 3).
- **Both binning rules copy and re-sort the data** when they build a new `summaryStatistics` inside.
- **No single `summary()` call** that returns every statistic at once.

## 6. Who changed it

- **Melissa:** `summaryStatistics` and Freedman–Diaconis binning (#54, #72), the `binningStrategy` classes (#83), and `kde()` with Silverman's bandwidth (#107). The first version of `kde.cpp` and `kde.hpp` was generated with Claude, then studied (Wikipedia, GeeksforGeeks) so it could be explained.
- **Oscar, with Claude Code:** fixed `kde.hpp` so it compiled, wrote `tests/test_kde.cpp` and added the files to CMake (#107, #108). He also made the end-of-project fixes: NaN and infinity handling, the 1,000-bin cap, the Sturges fallback and the equal-values case (`docs/bug-report.md`, bugs 1 to 4).
- **Norah:** the numeric-type tests for the statistics (#110).
- **This guide** was written with Claude, from the code, the tests and the project's own docs.
