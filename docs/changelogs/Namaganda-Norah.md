# Changelog: Namaganda Norah Magret

**Module:** Image export
**GitHub:** @NamagandaNorah-Margret20

<!-- Newest entries at the top. Add an entry in the same PR as the change. -->
## 2026-10-23
- **Changed:** Added `save_png(const Canvas&, path)` and `save_ppm(const Canvas&, path)` in `include/vizlib/canvas_export.hpp` and `src/canvas_export.cpp`, so a whole Canvas can be saved in one call. Added `tests/test_canvas_export.cpp` and registered the new source and test in both `CMakeLists.txt` files.
- **Why:** Examples such as `histogram_example.cpp` and `scatter_example.cpp` copy the canvas into a byte array by hand while waiting for this adapter. The PNG version keeps transparency (RGBA); the PPM version is RGB only.
- **PR:** #61
- **Tested:** 2 new doctest cases check the PNG signature and size, and the PPM size and pixel order (top-left first). All 29 test cases pass in `vizlib_tests.exe` (MSVC, Windows).

## 2026-10-14
- **Changed:** Added `save_png()` (using stb_image_write) and `save_ppm()` in `include/vizlib/image_export.hpp` and `src/image_export.cpp`. Added `third_party/stb_image_write.h` and `tests/test_image_export.cpp`. Registered the new source and the new test in both `CMakeLists.txt` files.
- **Why:** Week 2 task: the library needs to save a rendered picture as an image file. The functions take plain bytes (8-bit, row by row from the top-left), so they do not depend on the Canvas class.
- **PR:** #48
- **Tested:** doctest tests check the PNG signature and stored width and height, the exact PPM file size, and that bad input is rejected. All tests pass in `vizlib_tests.exe` (MSVC, Windows).