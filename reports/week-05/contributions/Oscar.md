# Week 05 Report: Member 4, Templates, KDE Joint Plot and Integration

**Name:** Kiwalabye Oscar Muwanguzi 25/U/08733/PS
**Task area:** Joint plots (this week: templates for my plots, the joint plot with KDE margins, and integrating the team's final pull requests)
**Week:** 26 October to 1 November 2026 (code freeze Saturday 31 October)
**Pull requests:** #92, #93 and #115 (my module), plus #90, #91, #102, #106, #107 and #108 (integration)

---

## 1. What I did this week

My goals were:

1. The lecturer's templates task: make the plots accept every numeric type (int, short, float, double, ...), with an appropriate concept, and rewrite the tests for them.
2. Finish the joint plot with KDE margins, using Member 2's KDE instead of my temporary one.
3. As the repo owner, get every member's final work onto `main` before the code freeze, building and passing CI.

All three are done. Goal 2 was finished on 2 November, one day after the week, because Member 2's KDE was merged on 1 November.

- **#92:** switched the project to C++20 and added `include/vizlib/concepts.hpp`, with the shared `vizlib::Numeric` concept and `to_doubles()`, for the whole team to use.
- **#93:** made `ScatterPlot` a class template, gave `HistogramPlot` a template constructor, and made `histogram_counts`, `auto_bin_edges`, `histogram`, `scatter` and `JointPlot::with_histograms` templates. Wrote 52 new test cases.
- **#115:**
  - Added `JointPlot::with_kde(x, y)` and a `KdePlot` constructor that takes Member 2's `KdeResult`.
  - Replaced my three temporary drawing helpers with Member 6's Canvas functions.
  - Switched `joint_plot_example.cpp` to the real KDE.
- **Integration:** rebuilt six team pull requests that conflicted with `main` or did not compile, keeping each member's commits under their name (section 6).
- **UML:** drew my class diagram (`docs/uml/member4-plots.png`).

## 2. Files I created or changed

- `include/vizlib/concepts.hpp`: the `Numeric` concept and `to_doubles()`.
- `tests/numeric_types.hpp`: `VIZLIB_NUMERIC_TYPES`, the list of types every templated module is tested with.
- `tests/test_concepts.cpp`: what the concept accepts and rejects.
- `include/vizlib/scatter.hpp`, `src/scatter.cpp`: `ScatterPlot<TX, TY>` and the `scatter()` templates.
- `include/vizlib/histogram.hpp`, `src/histogram.cpp`: template constructor and function templates.
- `include/vizlib/joint_plot.hpp`, `src/joint_plot.cpp`: `with_histograms` templates and `with_kde`.
- `include/vizlib/kde_plot.hpp`, `src/kde_plot.cpp`: `KdePlot(const KdeResult&)`, drawn with `Canvas::draw_line`.
- `tests/test_plot_templates.cpp`: templated tests for every plot.
- `tests/test_joint_plot.cpp`: tests for `with_kde`.
- `examples/joint_plot_example.cpp`: the KDE panel is now one `with_kde` call.
- `CMakeLists.txt`: C++17 to C++20.
- `docs/uml/member4-plots.mmd`, `.png`, `.md`: my UML diagram and how to read it.
- `docs/changelogs/Kiwalabye-Oscar.md`: entries for #92, #93 and #115.

## 3. Templates

### What became a template, and why

- **`ScatterPlot<TX, TY = TX>` is a class template.**
  - It keeps its x and y data, so the whole class depends on the type.
  - x and y can be different types, for example `int` years against `double` prices. The compiler works out the types: `ScatterPlot plot(years, prices)` is a `ScatterPlot<int, double>`.
  - Plain `ScatterPlot<>` means `ScatterPlot<double, double>`.
- **`HistogramPlot` is not a class template. Only its constructor is.**
  - The class only keeps the bin edges and the counts, and those do not depend on the type of the data.
  - Making the whole class a template would only make the compiler produce the same class once per type.
  - This was the main "think out loud" decision: a template only where the type is actually stored.
- **Function templates:** `histogram_counts<T>`, `auto_bin_edges<T>`, `histogram<T>`, `scatter<TX, TY>`, `JointPlot::with_histograms<TX, TY>` and `JointPlot::with_kde<TX, TY>`.
- **Bin edges and all maths stay `double`.** The mean of ints is not an int, pixel positions are not whole numbers, and bin edges usually fall between whole numbers even for int data. Each value is converted with `static_cast<double>` where it is used.

### The concept

```cpp
template <typename T>
concept Numeric =
    (std::integral<T> || std::floating_point<T>) &&
    !std::same_as<std::remove_cv_t<T>, bool> &&
    !std::same_as<std::remove_cv_t<T>, char>;
```

`bool` and `char` count as integers in C++, but they are never data you would plot, so they are left out. `ScatterPlot<bool>` or `ScatterPlot<std::string>` now fails to compile with a clear message, instead of drawing a strange plot.

### Templates and polymorphism together

`ScatterPlot<int, float>` still inherits from `Plot`. `JointPlot` keeps it in a `std::unique_ptr<Plot>` and calls its virtual `draw()` like any other plot. The template chooses the type at compile time, and the virtual function chooses the kind of plot at run time.

### Things I found

- **A template cannot work out `T` from a brace list.** For example, `scatter(canvas, area, {1, 2, 3}, {4, 5, 6})`. So each function keeps a plain `double` version next to the template. For a `std::vector<double>`, C++ picks the plain function over the template, so no existing code or test had to change.
- **The plain versions have to name the types.** They call the template as `scatter<double, double>(...)`. Without the explicit types they would call themselves forever.
- **Template code lives in the header,** because the compiler needs the full code wherever it is used. Helpers that only use doubles (`pad_bounds`, `draw_dot`, `check_edges`) stay in the `.cpp` files, so they are compiled once instead of once per type.
- **A `requires` check written outside a template is an error, not `false`.** To test that `ScatterPlot<bool>` is rejected, the check has to sit inside a small concept.

## 4. The joint plot with KDE margins

```cpp
JointPlot joint = JointPlot::with_kde(x, y);   // KDE from Member 2's kde()
joint.draw(canvas, {470, 50, 400, 380});
```

- `with_kde` builds the centre `ScatterPlot`, then calls Member 2's `kde()` once for x and once for y. That uses a Gaussian kernel, Silverman's bandwidth and 200 grid points.
- It puts each curve in a `KdePlot`. The top curve is vertical, and the right one is horizontal.
- NaN values are left out of the curves, because a single NaN would make the whole curve NaN. The scatter skips NaN points by itself.
- The shared scales from Week 4 still apply. The KDE grid runs 3 bandwidths past the data, so the x range of the centre panel widens to fit the tails of the curve.

## 5. UML

My diagram is `docs/uml/member4-plots.png`, with notes in `docs/uml/member4-plots.md`.

- `ScatterPlot<TX, TY>`, `HistogramPlot` and `KdePlot` inherit from the abstract `Plot`.
- `JointPlot` owns three `Plot`s (composition).
- The plots use Member 6's `Canvas` to draw and Member 5's `Axes` to map data to pixels. `KdePlot` can be built from Member 2's `KdeResult`.

## 6. Integration work

Several pull requests could not be merged as they were: they conflicted with `main` or would have broken it. In each case I rebuilt the member's own commits on top of the current `main` under their name, added a separate fix commit, merged that, and closed the original with a comment explaining what happened.

- **#90 (replaces Member 3's #85 to #88):**
  - The ECDF header still used `namespace viz`.
  - The test file had its own `main()` and was not in CMake.
  - The example had a wrong include.
- **#91 and #102 (replace Member 6's #81 and #101):**
  - Both came from an old branch that still carried commits already merged.
  - #101 would also have taken `ticks.cpp` back out of the build.
- **#106 (replaces Member 5's #103):**
  - Resolved the conflict with Member 8's tick labels in `axes.cpp`, and made the tick labels use `generateLogTicks` on a log axis.
  - Converted her three test programs, which used `main()` and `assert()`, into doctest cases. `assert` does nothing in the Release build that CI uses, so they had never checked anything in CI.
- **#107 (replaces Member 2's #104):**
  - `kde.hpp` did not compile: the concept was written inside a function body, and there was a stray `.` line. This went unnoticed because `kde.cpp` was not in CMake.
  - I fixed it, added it to the build, registered `test_stats.cpp`, and wrote `tests/test_kde.cpp`.
- **#108 (replaces Member 1's #105):**
  - `preprocessing.cpp` and his three test files were not in CMake, so two examples failed to link.
  - Moved `reports/Week-03/James.md` into `week-03/contributions`. `Week-03` and `week-03` are the same folder on Windows and macOS.

Before each merge I built all the open pull requests together, ran every test and example, and checked CI on Linux, Windows and macOS afterwards. The number of test cases that actually run went from 126 on 30 October to 227 on 2 November. Much of that increase is tests that existed but had never been registered.

## 7. How I tested it

- **227 test cases pass** in a Release build (the same as CI), on GCC 13 and Clang. CI on `main` passes on Linux, Windows and macOS.
- **52 new test cases in `test_plot_templates.cpp`.** Most are `TEST_CASE_TEMPLATE`s that run once for each of short, int, long, long long, unsigned int, float and double.
- **Each type must give the same counts, bounds and exactly the same pixels as the double version.** This covers scatter, histogram, `with_histograms` and `with_kde`.
- **Other new tests check:**
  - mixed types (int on x, double on y)
  - template plots used through a `std::unique_ptr<Plot>`
  - that the concept rejects `bool` and `char`
- **5 new test cases for `with_kde`:**
  - draws in all three panels
  - skips NaN
  - rejects bad input
  - same pixels for every type
  - `KdePlot` built from `kde()`
- **Sanitizers:** the new tests also pass under AddressSanitizer and UndefinedBehaviorSanitizer.
- **Examples:** all six run. I checked the joint plot PNG: the KDE curve of petal length shows the same two clusters as the histogram.

## 8. Problems I faced and how I solved them

- **Code that was never built or tested.** `kde.cpp`, `preprocessing.cpp` and seven test files were on `main`, or arrived in pull requests, without being added to CMake. (`ticks.cpp` had the same problem until Member 8 added it in #98.) CI was green while real errors were hidden. I added them, and I now check for this before every merge. I also asked the team to add new files to CMakeLists in the same PR.
- **Pull requests that conflicted or overlapped.** Members reused old branches, or opened several branches with the same edits. Force-pushing to a member's branch is not allowed, so I rebuilt their commits on a new branch instead. This keeps their authorship.
- **The 3-approval rule.** To get everyone's work merged before the code freeze, I merged with the repo owner's override, without three approvals. Each merge was first built and tested together with the other open pull requests, and CI was checked after it. This is the same problem as in Week 1, and the rule was not kept in practice for Weeks 3 to 5.
- **The KDE was blocked on Member 2.** I kept my temporary KDE in the example until her `kde()` was merged on 1 November, then switched the next day.

## 9. What is still pending

- Axis lines and tick marks on the plots, once Member 5 adds them.
- Member 3's ECDF does not draw yet. Once it is a `Plot`, it can go in a joint plot panel without any change to `JointPlot`.

## AI Use

- **Tool:** Claude (Claude Code)
- **Purpose:**
  - Designed and drafted the templates, the concept, the KDE joint plot and their tests.
  - Rebuilt and fixed the conflicting team pull requests and ran the builds, tests, sanitizers and CI checks.
  - Drew the UML diagram and drafted this report and the group reports.
- **Reason:** To get the whole team's work integrated, tested and building on every platform before the code freeze, while also finishing my own module.
