# Changelog: Namaganda Norah Magret

**Module:** Image export
**GitHub:** @NamagandaNorah-Margret20

<!-- Newest entries at the top. Add an entry in the same PR as the change. -->
## 2026-10-28
- **Changed:** `.github/workflows/ci.yml`: the `linux` job now runs `./build/examples/pipeline` after the tests and then checks that `data/output/pipeline.png` exists and is not empty (`test -s`). `examples/pipeline.cpp` now reads `data/input/iris.csv` with `read_csv` instead of generating random data, using a new helper `column_as_doubles` that turns a `DataFrame` column into a `std::vector<double>` (`sepal_length` for x, `petal_length` for y).
- **Why:** Week 4 tasks: complete GitHub Actions CI, test the build and tests on a clean environment, and run the complete CSV-to-PNG pipeline. Running `pipeline` in CI on a fresh machine also checks that no required files (the font, `data/input`, `data/output`) are missing from the repo.
- **PR:** #80
- **Tested:** Rebuilt with MSVC; `pipeline.exe` read `iris.csv` and saved `data/output/pipeline.png` with both plots and titles. `CI / linux` passes on the pull request (build, CTest, pipeline run and PNG check).
## 2026-10-26
- **Changed:** `examples/pipeline.cpp` now gets its histogram bin edges from `summaryStatistics::histogram_bin_edges` (data sorted first with `sorted`) and draws a title above each plot with `TextRenderer`. Fixed `include/vizlib/stats.hpp` and `src/stats.cpp`, which defined two different `summaryStatistics` classes (one global, one inside `vizlib`), so the linker could not find `histogram_bin_edges`. The header now holds the single class inside `namespace vizlib`, with `#pragma once`. In the root `CMakeLists.txt`, added `src/stats.cpp` and `ecdf.cpp` to the library and removed duplicate source lines; removed duplicate test lines in `tests/CMakeLists.txt`.
- **Why:** Week 3 task: integrate the different modules into the pipeline. The stats code is now usable, so the pipeline no longer needs hand-made equal-width bins.
- **PR:** #61
- **Tested:** Rebuilt with MSVC; `pipeline.exe` saved `data/output/pipeline.png` with both plots and titles. All tests pass in CTest.
## 2026-10-23
- **Changed:** Added `save_png(const Canvas&, path)` and `save_ppm(const Canvas&, path)` in `include/vizlib/canvas_export.hpp` and `src/canvas_export.cpp`, so a whole Canvas can be saved in one call. Added `tests/test_canvas_export.cpp` and registered the new source and test in both `CMakeLists.txt` files.
- **Why:** Examples such as `histogram_example.cpp` and `scatter_example.cpp` copy the canvas into a byte array by hand while waiting for this adapter. The PNG version keeps transparency (RGBA); the PPM version is RGB only.
- **PR:** #61
- **Tested:** 2 new doctest cases check the PNG signature and size, and the PPM size and pixel order (top-left first). All 29 test cases pass in `vizlib_tests.exe` (MSVC, Windows).


- **Changed:** Added `examples/pipeline.cpp` and registered it in `examples/CMakeLists.txt`. It generates sample data, draws a scatter plot and a histogram side by side, and saves one PNG with the Canvas export adapter.
- **Why:** Week 3 task: a first version of the pipeline that connects the modules. It uses generated data and equal-width bins for now, because the CSV loader and the stats code are not usable on `main` yet.
- **PR:** #61
- **Tested:** Built with MSVC and ran `pipeline.exe`. It saved `data/output/pipeline.png`, and the picture shows both plots correctly.

## 2026-10-14
- **Changed:** Added `save_png()` (using stb_image_write) and `save_ppm()` in `include/vizlib/image_export.hpp` and `src/image_export.cpp`. Added `third_party/stb_image_write.h` and `tests/test_image_export.cpp`. Registered the new source and the new test in both `CMakeLists.txt` files.
- **Why:** Week 2 task: the library needs to save a rendered picture as an image file. The functions take plain bytes (8-bit, row by row from the top-left), so they do not depend on the Canvas class.
- **PR:** #48
- **Tested:** doctest tests check the PNG signature and stored width and height, the exact PPM file size, and that bad input is rejected. All tests pass in `vizlib_tests.exe` (MSVC, Windows).
