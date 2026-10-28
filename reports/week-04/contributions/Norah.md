# Week 4 Report: vizlib C++ Data Visualization Library

**Author:** Namaganda Norah Margret 
**Week:** 4  
**Pull Request:** #80

## 1. Goals for the Week

- Complete GitHub Actions CI.
- Test the project in a clean environment.
- Fix build and integration problems.
- Run the complete CSV-to-PNG pipeline.
- Verify that required files and dependencies are tracked.

## 2. Work Completed

### 2.1 GitHub Actions CI

The workflow in `.github/workflows/ci.yml` builds the project and runs CTest on Linux for pushes and pull requests to `main`. A second job checks Windows and macOS after pushes to `main`.

I checked the CMake testing configuration and added steps to run the pipeline and verify that `data/output/pipeline.png` exists and is not empty.

### 2.2 CSV-to-PNG Pipeline

I updated `examples/pipeline.cpp` to read `data/input/iris.csv` instead of generating random data. The `column_as_doubles()` helper extracts numerical values from the `DataFrame`.

The pipeline uses `sepal_length` and `petal_length` for the scatter plot and calculates histogram bin edges using `summaryStatistics`. It draws both plots on a canvas, adds titles using `TextRenderer`, and saves the result as `data/output/pipeline.png`.

### 2.3 Files and Dependencies

I used `git ls-files data third_party/fonts` to verify that the Iris and tips datasets, DejaVu Sans font, and output-directory `.gitkeep` files are tracked.

## 3. Results

- The pipeline builds and runs on my Windows machine, producing a PNG containing a scatter plot and histogram.
- The Linux CI job passed on Pull Request #80.
- Windows and macOS CI results still need to be confirmed from GitHub.

## 4. Problems and Fixes

I checked the CMake configuration and removed duplicate source-file entries. I also checked the CI workflow and added a pipeline execution step with an output-file check. Replacing randomly generated data with the tracked Iris dataset made the pipeline more suitable for repeatable testing.

## 5. Classes Used in the Pipeline

### 5.1 DataFrame

The `DataFrame` class stores CSV data in columns. Its private members are `columns_` and `rows_`. Public functions include `add_column()`, `get_column()`, `row_count()` and `column_count()`.

In the pipeline, `read_csv()` loads the Iris dataset, while `column_as_doubles()` extracts numerical values from the required columns.

### 5.2 Canvas

The private members `width_`, `height_` and `pixels_` store the canvas dimensions and pixel colours. Public functions include `set_pixel()`, `get_pixel()`, `draw_line()` and the rectangle and circle drawing functions.

The pipeline creates a 700 × 300 pixel canvas and uses it to draw both plots and their titles.

### 5.3 Axes

The private members store the x- and y-axis limits, plot area and linear scaling objects. Public functions include `setDataRange()`, `setPlotArea()`, `dataToPixel()`, `pixelToData()` and `getPlotArea()`.

The pipeline uses `setPlotArea()` to assign the histogram's drawing area.

### 5.4 TextRenderer

The private member `impl_` manages the hidden implementation. Public functions include `measure_text()` and `draw_text()`.

The pipeline loads `DejaVuSans.ttf` and uses `draw_text()` to add the titles “Scatter plot” and “Histogram”.

### 5.5 summaryStatistics

The private vectors `Data` and `Sorted_data` store the original and sorted values. Public functions calculate the mean, median, range, mode, standard deviation, quantiles and other statistics.

The pipeline uses `histogram_bin_edges()` to obtain the intervals needed for the petal-length histogram.

### 5.6 Plotting and Export

The `scatter()` function draws the scatter plot, while `histogram()` draws the histogram. The `save_png()` function exports the completed canvas to `data/output/pipeline.png`.

## 6. Next Steps

- Obtain approval and merge Pull Request #80.
- Confirm that Windows and macOS CI pass.
- Check the final build, tests and required files before the 31 October code freeze.

## 7. References

1. GitHub Actions documentation: https://docs.github.com/actions
2. CMake documentation: https://cmake.org/cmake/help/latest/
3. `stb_image_write.h`: https://github.com/nothings/stb
4. DejaVu Fonts: https://dejavu-fonts.github.io/
5. Fisher, R. A. (1936). *The use of multiple measurements in taxonomic problems*. Annals of Eugenics, 7(2), 179–188. https://doi.org/10.1111/j.1469-1809.1936.tb02137.x

## AI Use Declaration

This report was prepared with assistance from ChatGPT.
