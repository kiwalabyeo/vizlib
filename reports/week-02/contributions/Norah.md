# Week 2 Report: Image Export

**Name:** Namaganda Norah Margret 25/U/0846
**Task area:** Image export (saving a canvas as an image file)
**Branch / pull request:** `feature/image-export`

---

## 1. What I did this week

My task for Week 2 was to make the library able to save pictures as image files. I did the following:

- Checked the project files and build instructions, and built the project before making changes.
- Agreed on the format used to store pixel colours (see Section 4).
- Created the files needed for image export.
- Implemented `save_png()` to save images as PNG files using `stb_image_write`.
- Implemented `save_ppm()` to save images as PPM files.
- Wrote tests in `tests/test_image_export.cpp` to check that the files are saved correctly.
- Started working on an adapter to save a complete `Canvas` directly. This part is not yet committed because my partner's Canvas implementation has not yet been merged into `main`.
- Opened a pull request for the image export code and tests.

## 2. Files I created or changed

| File                                        | Purpose                                               |
| ------------------------------------------- | ----------------------------------------------------- |
| `include/vizlib/image_export.hpp`           | Declares the `save_png()` and `save_ppm()` functions. |
| `src/image_export.cpp`                      | Contains the code for saving PNG and PPM files.       |
| `third_party/stb_image_write.h`             | Library used to write PNG files.                      |
| `tests/test_image_export.cpp`               | Tests the image export functions.                     |
| `CMakeLists.txt` and `tests/CMakeLists.txt` | Updated to include the new code and tests.            |

**Not yet committed:** `include/vizlib/canvas_export.hpp`, `src/canvas_export.cpp`, and `tests/test_canvas_export.cpp`.

These files are still on my computer because my partner's Canvas implementation has not yet been merged into `main`. I need that code to complete and test the Canvas adapter properly.

## 3. How the export works

**`save_png(filename, width, height, channels, pixels)`**

This function takes the image's size, colour information and pixel data, then saves the image as a PNG file. It returns `true` if saving succeeds and `false` if something is wrong. It rejects invalid sizes, unsupported channel counts and missing pixel data.

**`save_ppm(filename, width, height, channels, pixels)`**

This function saves an image as a PPM file. PPM is a simple image format with a short text header followed by the colour data. My function accepts only three colour values per pixel: red, green and blue.

Both functions use pixel data directly instead of depending on the `Canvas` class. This makes them easier to use with different parts of the library.

**The Canvas adapter**

The `Canvas` class stores its pixels privately, so my export functions cannot access them directly. I started working on two extra functions that save a `Canvas` as a PNG or PPM file.

These functions read the pixels using `get_pixel(x, y)`, copy their colour values into an array, and pass the array to the export functions. PNG keeps all four colour values, including transparency, while PPM uses only red, green and blue.

## 4. The pixel format

From my partner's rendering code, I found that:

- Each pixel has four colour values: red (`r`), green (`g`), blue (`b`) and alpha (`a`).
- Each value ranges from 0 to 255.
- Pixels are stored row by row, starting from the top-left corner.

Therefore, the agreed format is **8-bit RGBA, stored row by row from the top-left**. This allows the image to be saved in the correct orientation.

## 5. How I tested it

I used `doctest`, which was already part of the project. My tests check that:

- A saved PNG file exists, is not empty, has the correct PNG signature, and stores the correct width and height.
- A saved PPM file has the expected size, including its header and pixel data.
- Invalid inputs, such as zero width, missing pixel data or an incorrect channel count for PPM, are rejected.
- The Canvas adapter saves the correct pixel data and places the top-left pixel first. This test is still part of my local work.

All the image export tests pass when I run `vizlib_tests.exe`.

## 6. Problems I faced and how I solved them

- **My test had its own `main()`:** The project uses `doctest`, which provides the test program's `main()`. I changed my test to use `TEST_CASE` blocks and added it to `tests/CMakeLists.txt`.
- **CMake was not installed:** I installed CMake and restarted VS Code.
- **No C++ compiler was available:** CMake could not find a compiler, so I installed Visual Studio Build Tools with the *Desktop development with C++* workload.
- **CTest could not run the tests:** It reported `BAD_COMMAND`, but running `vizlib_tests.exe` directly worked and the tests passed. I have not yet confirmed the cause.
- **Git asked whether to try again when deleting folders:** This may be related to the project being stored in OneDrive. I chose not to retry.
- **The Canvas implementation was missing from `main`:** My partner's `canvas.hpp` was available, but `canvas.cpp` had not yet been merged. I used a local copy of the implementation for testing. I have not committed the Canvas adapter because I am waiting for my partner's changes to be merged into `main`.

## 7. Class structure

### Did i create any class?

No. My main image export code uses ordinary functions: `save_png()` and `save_ppm()`. I chose functions because their job is simply to take pixel data, save it to a file, and return whether the operation succeeded. They do not need to store information between calls.

My work also uses two types created by my partner: `Canvas` and `Color`.

### `Canvas`&#x20;

**What is it?**

`Canvas` represents the picture being drawn. It stores the pixels and provides functions for drawing shapes.

**Private data members:**

- `width_`: the number of pixels across.
- `height_`: the number of pixels down.
- `pixels_`: a `std::vector<Color>` containing all the pixel colours.

**Public member functions:**

- The constructor creates a canvas with a given size and background colour.
- `width()` and `height()` return the canvas dimensions.
- `set_pixel()` and `get_pixel()` change or read individual pixels.
- `draw_line()`, `draw_rect()`, `fill_rect()`, `draw_circle()` and `fill_circle()` draw shapes.

**Why are some members private and others public?**

The pixel data is private to prevent other code from changing it directly. Public functions allow other parts of the program to use the canvas safely. For example, `set_pixel()` ignores coordinates outside the canvas, while `get_pixel()` reports an error for invalid coordinates.

**How my code uses it:** My adapter uses `width()`, `height()` and `get_pixel()` to read the canvas and prepare the pixel data for saving.

### `Color` (a struct)

**What is it?**

`Color` stores the colour of one pixel using four values: `r`, `g`, `b` and `a`. Each value is an `unsigned char` ranging from 0 to 255. The default colour is opaque black.

**Why is it a struct?**

A `Color` simply stores four values, so its members are public and can be accessed directly. A class with private members is not necessary for this simple purpose.

**How my code uses it:** My adapter reads the four colour values from each pixel and copies them into the array used by the export functions.

## 8. What is still pending

- My pull request is waiting for three approvals before CI runs.
- My partner's Canvas implementation still needs to be merged into `main`. I have asked my partner to open a pull request for it and am waiting for a response.
- Once the Canvas implementation is merged, I will update my branch, finish testing the adapter, commit the remaining files and push them to GitHub.
