
# Week 02 Report: Member 6, Plot Rendering

Name: Nambalirwa Flavia Cate
Reg No: 25/U/0848
Programme: BELE
Group 7, vizlib

## Completed

* Wrote the three header files for the rendering part, in `include/vizlib/`: `color.hpp`, `geometry.hpp` and `canvas.hpp`. They are in a pull request (#__) for review. Everything is inside `namespace vizlib` and each file uses `#pragma once`.
* `color.hpp`: a struct `Color` with four `unsigned char` members `r`, `g`, `b`, `a` (0 to 255). `a` is alpha, the opacity: 255 is fully solid, 0 is invisible. The default is opaque black.
* `geometry.hpp`: a struct `Point` (`x`, `y`) and a struct `Rect` (`x`, `y` of the top-left corner, plus `width` and `height`). A line is drawn between two Points, and `Rect` is used for the rectangle functions.
* `canvas.hpp`: a class `Canvas` that holds the pixels. Private members are `width_`, `height_` and a flat `std::vector<Color> pixels_`. The pixel at (x, y) is stored at index `y * width + x`. Public functions: constructor, `width()`, `height()`, `set_pixel()` and `get_pixel()`. `set_pixel` ignores points outside the canvas. `get_pixel` throws an exception for them.
* Used headers (`.hpp`) with matching `.cpp` files, as in the rest of the C++17 project, so the other members can include my interface without seeing my implementation.
* Learned how the line algorithms work: DDA (step along the longer axis and round) and Bresenham (integer error term, no rounding).

## Why Color, Point and Rect exist

The structures (struct) are used to bundle related variables together into a single package to make code cleaner, safer, and easier to read.
Each one groups values that always belong together. A colour is four numbers (red, green, blue, alpha), and a point is two. If they were loose variables, every function would have to take four or two or even more separate arguments, take for example "void draw_rect(int x, int y, int width, int height, int r, int g, int b, int a);" and it would be easy to swap their order. A single type means you pass one thing around. 
Also, a safety precatution was odne by making the default colour opaque black so that a Color never holds random leftover values. Rect bundles a position and a size, so draw_rect and fill_rect take one argument instead of four or more.

## Why their members are public

These are plain data holders. They have no rules to protect: any x, y or red value is a valid value, so letting other code set them freely can’t break anything. That is why struct fits, since struct members are public by default. If they have no member functions, say so in the report.

## Why Canvas is a class with private data

Canvas has a rule that must always stay true: the size of the vector has to equal width times height. If pixels_ were public, anyone could resize it or index it wrongly and corrupt the image or crash the program. Making the data private forces all access through the public functions. That is the main idea of the exercise.

## Why each public function is shaped the way it is

width() and height() let others read the size without being able to change it.
set_pixel ignores points outside the canvas, because a line that runs partly off the edge is normal and shouldn’t crash.
get_pixel throws, because reading a pixel that doesn’t exist is a mistake the caller should find out about.
The formula y * width + x is a hidden detail. Because it’s private, you could later change how pixels are stored without breaking anyone else’s code.

## How the rest of the program uses it

Canvas is the shared drawing surface. The drawing functions, such as draw_line and draw_rect, take a Canvas and call set_pixel to colour pixels. The plot code from other members would draw through those functions. Look at your group’s plan and your own code to say exactly who uses it.

## In Progress

* `src/canvas.cpp`: the bodies of the constructor, `set_pixel` and `get_pixel`.
* Unit tests (doctest): set a pixel and read it back, a zero-size canvas throws, `set_pixel` off the canvas does not crash.
* `draw_line`, starting with a simple DDA version.

## Challenges

* Deciding whether `Point` and `Rect` should hold `int` or `double`. I used `double` for now and will confirm it with the group.
* `get_pixel` throws `std::out_of_range`, but the group rule mentions `std::invalid_argument`. I will agree one choice with the group.
* Understanding how a smooth line is built from separate square pixels(the Bresenham line algorithm).A straight line on screen is really a staircase of pixels (aliasing).

## Next Week

* Get the headers PR approved and merged, then open a second PR with `canvas.cpp` and the tests.
* Finish `draw_line` (DDA, then Bresenham with credit to the source), then `draw_rect` and `fill_rect`.
* Plan the circle functions for week 3.

## AI Use

* Claude (Anthropic): explained pixels, the canvas layout, DDA and Bresenham, and header vs source files; reviewed my header files and pointed out mistakes (spelling of `Color`, an unclosed namespace, and the order of initialisation in the constructor), which I then fixed. I typed and understood the final code myself.



