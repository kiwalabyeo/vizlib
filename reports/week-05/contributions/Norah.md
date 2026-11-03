# Week 5 Report: vizlib C++ Data Visualization Library

**Author:** Namaganda Norah Margret  
**Week:** 5  

## 1. Goals for the Week

The main goal this week was to check whether the project was ready for submission. I wanted to make sure that someone downloading the repository for the first time could build it, run the tests and generate the expected output without missing files or dependencies.

I also worked on improving the README and expanding the statistics tests.

## 2. Work Completed

### 2.1 Checking the CI Workflow

I checked the GitHub Actions workflow in `.github/workflows/ci.yml` and confirmed that the latest runs on the `main` branch showed successful results.

This helped confirm that the automated build and testing process was working.

### 2.2 Fresh-Clone Test

I cloned the repository into a new folder to test it from the perspective of someone accessing the project for the first time.

I followed the usual steps:

1. Configured the project using `cmake -S . -B build`.
2. Built it using `cmake --build build`.
3. Ran the tests using `ctest --test-dir build`.
4. Ran the complete CSV-to-PNG pipeline.
5. Opened `data/output/pipeline.png` to check the generated image.

The project built successfully, the tests passed and the pipeline generated the expected PNG output. This was useful in checking that the project did not depend on files or settings from my previous development environment.

### 2.3 README and LICENSE Improvements

I created a pull request called `docs/readme-final` to improve the README and add a LICENSE file.

The aim was to make the repository easier to understand and follow, especially for someone who is accessing it for the first time.

### 2.4 Improving the Statistics Tests

I also worked on `tests/numeric-types-stats`, where I updated `test_stats.cpp`.

Previously, the statistics tests did not cover several numeric types. I used doctest's `TEST_CASE_TEMPLATE` to allow the same test to run for `int`, `short`, `float` and `double`.

I also introduced `VIZLIB_NUMERIC_TYPES` to define the types in one place instead of repeating them in different tests. This makes the tests easier to maintain and extend.

## 3. Results

By the end of the week:

- The project built successfully from a fresh clone.
- All tests passed.
- The CSV-to-PNG pipeline ran successfully.
- I checked the generated PNG output.
- The README and LICENSE improvements were submitted for review.
- The statistics tests were updated to cover four numeric types.

Both pull requests have successful CI checks and are currently waiting for the required team approvals.

## 4. Challenges and Solutions

One issue I noticed was that the repository did not have a LICENSE file, and the README needed some improvements. I addressed these through the `docs/readme-final` pull request.

Another area that needed improvement was the statistics testing. The tests needed to cover more than one numeric type. I used `TEST_CASE_TEMPLATE` to avoid writing almost identical tests separately for each type.

## 5. My Contribution This Week

My main focus this week was checking the project from a new user's perspective, improving the repository documentation and expanding the statistics tests.

I also checked that the generated PNG output was being produced correctly after running the complete pipeline.

## 6. Next Steps

- Obtain the required three team approvals for each pull request.
- Merge both pull requests into `main`.
- Run the build and tests again after merging.
- Confirm that the CI checks remain successful.
- Complete the final project submission.

## 7. References

1. GitHub Actions documentation: https://docs.github.com/actions
2. CMake documentation: https://cmake.org/cmake/help/latest/
3. doctest documentation: https://github.com/doctest/doctest

## AI Use Declaration

This report was prepared with assistance from AI tools. The technical work, testing and project contributions described above were carried out as part of my work on vizlib.
