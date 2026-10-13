# Changelog: Namaganda Norah Magret

**Module:** Image export
**GitHub:** @NamagandaNorah-Margret20

<!-- Newest entries at the top. Add an entry in the same PR as the change. -->

## 2026-10-14
- **Changed:** Added `save_png()` (using stb_image_write) and `save_ppm()` in `include/vizlib/image_export.hpp` and `src/image_export.cpp`. Added `third_party/stb_image_write.h` and `tests/test_image_export.cpp`. Registered the new source and the new test in both `CMakeLists.txt` files.
- **Why:** Week 2 task: the library needs to save a rendered picture as an image file. The functions take plain bytes (8-bit, row by row from the top-left), so they do not depend on the Canvas class.
- **PR:** #48
- **Tested:** doctest tests check the PNG signature and stored width and height, the exact PPM file size, and that bad input is rejected. All tests pass in `vizlib_tests.exe` (MSVC, Windows).