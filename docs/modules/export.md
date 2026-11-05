# Export, pipeline and CI: saving the picture and checking it all works

- **Owner:** Namaganda Norah Magret (Member 7)
- **Headers:** `include/vizlib/image_export.hpp`, `canvas_export.hpp`
- **Sources:** `src/image_export.cpp`, `src/canvas_export.cpp`
- **Third party:** `third_party/stb_image_write.h`
- **Tests:** `tests/test_image_export.cpp` and `test_canvas_export.cpp` (5 test cases)
- **Example:** `pipeline` (CSV in, PNG out), and every other example, which saves its picture with `save_png`
- **Also hers:** `.github/workflows/ci.yml`, `README.md` and `LICENSE`

## 1. What it does

It is the last step of the pipeline. It saves a drawn `Canvas` as an image file:

- **PNG:** keeps transparency (RGBA). This is the format every example uses.
- **PPM (P6):** a very simple format, RGB only. Easy to check by hand.

```cpp
#include <vizlib/canvas_export.hpp>

vizlib::Canvas canvas(800, 600, vizlib::Color{255, 255, 255, 255});
// ... draw the plots ...
if (!vizlib::save_png(canvas, "data/output/plot.png")) {
    // the file could not be written
}
```

Norah also wrote `examples/pipeline.cpp`, which joins all the modules, and the CI that runs it on every pull request.

## 2. How it works

**Two layers.**

1. `image_export` takes plain bytes: width, height, the number of channels, and a pointer to the pixels, row by row from the top-left. It knows nothing about `Canvas`.
2. `canvas_export` copies a `Canvas` into a byte array (4 bytes per pixel for PNG, 3 for PPM) and calls the first layer.

**PNG.** `save_png` calls `stbi_write_png` from stb_image_write. That library does the compression. The row size passed to it is width × channels.

**PPM.** `save_ppm` writes the header `P6`, then the width and height, then `255`, the largest colour value. Then it writes all the pixel bytes. So a 2 × 2 picture is exactly `P6\n2 2\n255\n` (11 bytes) plus 12 bytes of pixels.

**Bad input.** A width or height of 0 or less, a wrong number of channels, or a null pointer returns `false` and writes nothing. A file that cannot be opened also returns `false`.

**The pipeline** (`examples/pipeline.cpp`): read `data/input/iris.csv` → take two columns → work out the histogram bins with Member 2's stats → draw a scatter and a histogram with axes, titles and labels → save `data/output/pipeline.png`.

**CI** (`.github/workflows/ci.yml`): on every pull request, a fresh Linux machine configures with CMake, builds, runs all the tests with CTest, runs `pipeline`, and checks that `pipeline.png` exists and is not empty (`test -s`). On a push to `main` the same runs on Windows and macOS too.

## 3. Why it is written this way

- **Two layers, so export does not depend on drawing.** `image_export` was written in Week 2, before `Canvas` was ready. Keeping it on plain bytes meant it could be built and tested on its own. `canvas_export` is a small adapter added later (#61).
- **Overloading.** There are two `save_png` functions: one takes bytes, the other a `Canvas`. The compiler picks the right one from the arguments.
- **Return `bool`, not throw.** Not being able to save a file is a normal thing to handle, like a full disk or a missing folder. So the caller checks the result.
- **`std::filesystem::path`.** File names work the same way on Windows, Linux and macOS.
- **`const Canvas&`.** Saving only reads the canvas, so it is passed by `const` reference and never copied.
- **stb_image_write, not our own PNG code.** PNG needs compression and checksums, which is a lot of work to get right. stb is one header file, public domain, and needs no install.
- **The pipeline runs in CI.** It runs on a clean machine, so it also checks that no files the repo needs (the font, `data/input`, `data/output`) are missing.

## 4. How it is tested

- **PNG:** the file starts with the 8-byte PNG signature, and the width and height stored inside are correct.
- **PPM:** the file size is exact, and the first pixel in the file is the top-left pixel.
- **Bad input:** a size of 0, a wrong channel count and a null pointer all return `false`.
- **Canvas adapter:** a small canvas is saved both ways and checked the same way.
- **The whole pipeline:** run in CI on every pull request, and on all 3 systems when `main` changes.

## 5. Limitations

- Only PNG and PPM. No JPEG or SVG.
- PPM drops transparency.
- `save_png` returns `false` but does not say why it failed.
- The output folder must already exist. It is not created.

## 6. Who changed it

- **Norah:**
  - `save_png` and `save_ppm` with their tests (#48, merged 15 October)
  - the `Canvas` adapter and the first version of the pipeline (#61, merged 25 October)
  - the pipeline with the stats and the titles, and the fix for the two `summaryStatistics` classes (#74, merged 27 October)
  - the pipeline reading `iris.csv`, and CI running it and checking the PNG (#80, merged 29 October)
  - the README and the MIT `LICENSE` (#109), and the stats tests for 7 number types (#110), merged 1 November
- **Oscar:** set up the first CI (2 October). On 4 November, with Claude Code, he made Windows and macOS also run the pipeline and check the PNG, and fixed the folder where Windows puts the programs.
- **This guide** was written by Member 4 with Claude Code, from Norah's changelog and her code.
