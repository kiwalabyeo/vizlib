# Canvas: the picture every plot is drawn on

- **Owner:** Nambalirwa Flavia Cate (Member 6)
- **Headers:** `include/vizlib/canvas.hpp`, `color.hpp`, `geometry.hpp`
- **Source:** `src/canvas.cpp`
- **Tests:** `tests/test_canvas.cpp` (19 test cases)
- **Example:** `shapes_example`, which draws every shape and saves `data/output/shapes_example.png`

## 1. What it does

`Canvas` is a grid of coloured pixels. Every other drawing module (plots, axes, text, legends) draws onto it, and Member 7's export saves it as a PNG.

- **`Color`:** red, green, blue and alpha (transparency), each from 0 to 255.
- **`Point`:** a position (x, y). **`Rect`:** a top-left corner plus a width and height.
- **`Canvas`:** `set_pixel`, `get_pixel`, `draw_line` (with a thickness), `draw_rect`, `fill_rect`, `draw_circle` and `fill_circle`.

```cpp
#include <vizlib/canvas.hpp>

vizlib::Canvas canvas(400, 300, vizlib::Color{255, 255, 255, 255});   // white
canvas.draw_line({10, 10}, {390, 290}, vizlib::Color{0, 0, 0, 255}, 2);
canvas.fill_circle({200, 150}, 40, vizlib::Color{31, 119, 180, 255});
```

## 2. How it works

**Storing the pixels.** The pixels are kept in one `std::vector<Color>`, row by row. The pixel at (x, y) is at position $y \times \text{width} + x$. The size is worked out as `std::size_t`, because as `int` a very large canvas would overflow.

**Lines: Bresenham's algorithm (1965).** It walks from one end of the line to the other, one pixel at a time, using only whole numbers. An error value keeps track of how far the pixels are from the true line. At each step it moves in x, in y, or in both (a diagonal step), whichever keeps the error small. For a thick line, each pixel is stamped as a small square of side `thickness`.

**Circles: the midpoint algorithm.** It works out one eighth of the circle and mirrors each point 8 ways, because a circle is symmetric.

**Filled shapes.** `fill_rect` colours every pixel inside the rectangle. `fill_circle` goes row by row. For a row dy away from the centre, the half-width is $\sqrt{r^2 - dy^2}$, so it colours the strip from cx − half to cx + half.

**Clipping.** `set_pixel` ignores positions off the canvas. On top of that, `fill_rect` and `fill_circle` limit their loops to the canvas, and `draw_line` skips a line whose two ends are both past the same edge. So a shape far off the canvas costs almost nothing.

## 3. Why it is written this way

- **Encapsulation.** The pixel vector, width and height are private. The constructor throws `std::invalid_argument` for a size of 0 or less, or more than 20000, so a `Canvas` always has a valid grid.
- **Structs for `Color`, `Point` and `Rect`.** They are plain data with no rules to protect, so everything is public. `unsigned char` (0 to 255) is exactly one colour channel and keeps a `Color` at 4 bytes.
- **Inputs are checked.** NaN, infinity and coordinates bigger than 1,000,000 throw, because they would otherwise turn into nonsense pixel positions. A thickness must be between 1 and 1000, and a radius must not be negative.
- **Empty rectangles draw nothing.** A histogram bar of height 0 is valid, so it should not stop the whole plot by throwing.
- **`get_pixel` throws, `set_pixel` does not.** Drawing a shape that is partly off the canvas is normal, so `set_pixel` quietly skips. Reading a pixel that does not exist is a mistake, so `get_pixel` throws `std::out_of_range`.
- **No templates.** Pixels are always whole numbers and colours are always 4 bytes, so a template would add nothing.
- **Shared types.** `Point` and `Rect` are used by every module, so all of them speak the same language.

## 4. How it is tested

- **Pixels:** set then get gives the same colour. Off-canvas positions are ignored by `set_pixel` and throw in `get_pixel`.
- **Shapes:** a horizontal and a diagonal line land on the right pixels. `draw_rect` and `draw_circle` are outlines only, `fill_rect` and `fill_circle` are solid. A thick line covers the rows above and below.
- **Edge cases:** lines, rectangles and circles partly or fully off the canvas do not crash and are clipped. A one-pixel rectangle draws one pixel. Empty rectangles draw nothing.
- **Bad input:** a canvas of size 0, a thickness below 1, a negative radius, and NaN, infinity or huge values all throw.

## 5. Limitations

- No anti-aliasing for lines and circles, so diagonal lines have jagged edges. Only text is smoothed (Member 8).
- New colours replace the old ones. Transparent colours are not blended on shapes.
- No dashed lines and no polygons.

## 6. Who changed it

- **Flavia:**
  - `Color`, `Point`, `Rect` and the `Canvas` header (9 October)
  - the Canvas drawing code and its tests (#81, 28 October), rebuilt by Oscar as #91 and merged on 29 October, keeping her commits
  - input checks, skipping invisible lines, clipping the filled shapes, more tests, and comments on the public functions in `canvas.hpp` (#101, 31 October), rebuilt by Oscar as #102 and merged the same day, keeping her commits
- **Oscar:** fixed the build after the Week 3 merges (#71). On 3 November, with Claude Code, he fixed a bug where a very large canvas overflowed an `int` (`docs/bug-report.md`, bug 5), and on 4 November added `shapes_example`.
- **This guide** was written by Member 4 with Claude Code, from Flavia's code, tests and reports.
