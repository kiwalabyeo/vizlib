# Text: titles, labels, tick numbers, legends and notes

- **Owner:** Kabahamba Joy A Maria (Member 8)
- **Headers:** `include/vizlib/text.hpp`, `drawable.hpp`, `legend.hpp`, `annotations.hpp`, and her parts of `figure.hpp` and `axes.hpp`
- **Sources:** `src/text.cpp`, `src/legend.cpp`, `src/annotations.cpp`, `drawLabels` in `src/figure.cpp`, `drawTickLabels` in `src/axes.cpp`
- **Third party:** `third_party/stb_truetype.h` and the font `third_party/fonts/DejaVuSans.ttf`
- **Tests:** `tests/test_text.cpp`, `test_annotations.cpp`, `test_legend.cpp`, `test_drawable.cpp` and `test_figure_labels.cpp` (14 test cases)
- **Example:** `pipeline`, which shows a title, axis labels, tick numbers, a legend and a note. It draws the legend and the note in one loop through `Drawable` pointers.
- **User guide:** `docs/text.md`

## 1. What it does

It draws every word on the plots: the title, the axis labels, the numbers under the ticks, the legend, and free-text notes (annotations).

```cpp
#include <vizlib/legend.hpp>
#include <vizlib/text.hpp>

const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
text.draw_text(canvas, "Hello", vizlib::Point{10, 30}, 16, vizlib::Color{0, 0, 0, 255});

vizlib::Legend legend;
legend.add_entry("Iris samples", vizlib::Color{31, 119, 180, 255});
legend.draw(canvas, text);
```

## 2. How it works

**Measuring text.** `stbtt_ScaleForPixelHeight` gives the scale from font units to pixels. For each letter, the width grows by the letter's advance width times the scale, plus the kerning between this letter and the next. Kerning is a small spacing fix for pairs like "AV". The height is (ascent − descent) × scale. For example, `measure_text("Hello", 12)` gives a width of 26.13 and a height of 12. At size 24 the width doubles exactly.

**Drawing text.** The point passed to `draw_text` is the **baseline**, the line the letters sit on, not the top-left corner. For each letter, stb_truetype gives a small greyscale bitmap. Each value in it is the *coverage*: how much of that pixel the letter covers, from 0 to 255. Each pixel is blended with what is already there:

  new = colour × alpha + background × (1 − alpha), where alpha = coverage / 255

Partly covered pixels become grey, which gives smooth letter edges (anti-aliasing).

**Legend.** Each entry is a 12 × 12 colour square, then its label 6 pixels to the right. The rows are max(font size, 12) + 6 pixels apart, so 18 pixels at size 12.

**Titles and labels** (`Figure::drawLabels`). The title is centred above the first plot area, and the x label below it. The text is measured first so it can be centred. The y label is drawn above the plot on the left, and kept inside the image with `std::max(5.0, …)`.

**Tick numbers** (`Axes::drawTickLabels`). A number is written under every x tick and left of every y tick. They use the same tick values as Member 5's tick marks, so each number sits under its mark. The text of each number comes from `formatTickLabel`.

## 3. Why it is written this way

- **`Drawable` is an abstract base class.** It has one pure virtual function, `draw(Canvas&, const TextRenderer&) const = 0`, and a virtual destructor. `Legend` and `Annotations` derive from it. Code can hold a mix of them as `Drawable` pointers and draw them all in one loop, and the right `draw` runs at run time. That is runtime polymorphism.
- **Override and overload in `Legend`.** `draw(canvas, renderer)` overrides the virtual function. `draw(canvas, renderer, fontSize)` is an overload: the same name with an extra parameter. The first calls the second with size 12.
- **The pimpl idiom in `TextRenderer`.** The header has only `struct Impl;` and a `std::unique_ptr<Impl>`. All the stb_truetype types live in `text.cpp`, so no other file depends on the font library. That is encapsulation of the implementation itself.
- **The destructor is defined in `text.cpp`.** `unique_ptr<Impl>` must know the full `Impl` type to delete it, and only `text.cpp` has that.
- **Move-only.** Copying is deleted, because a copy would need its own copy of the loaded font. Moving only hands the pointer over, so it is allowed. That is the rule of five, applied on purpose.
- **The constructor checks the font.** A missing or unreadable font file throws `std::runtime_error`. So a `TextRenderer` that exists can always draw.
- **No templates.** Text, fonts, colours and positions always use the same types (`std::string_view`, `double`, `Color`). See `docs/design-decisions.md`.

## 4. How it is tested

- **Text:** measuring gives a positive size, drawing changes pixels on the canvas, and a missing font file throws. On 3 November tests were added that refuse a NaN or infinite font size (`tests/test_edge_cases.cpp`).
- **Annotations and legends:** adding, reading back, clearing and drawing them. A font size of 0 or below is refused.
- **`Drawable`:** a `Legend` and an `Annotations` are drawn through `Drawable` pointers.
- **Figure labels:** the title and labels are stored and drawn, and nothing is drawn when they are empty.
- **Tick numbers** (`tests/test_untested_parts.cpp`): numbers appear below and left of the plot area, and none inside it, on linear and log axes.

## 5. Limitations

- The y-axis label is drawn horizontally above the axis, because text cannot be rotated yet.
- The font path is relative, so programs must be run from the project folder.
- Only one font is used, DejaVu Sans.

## 6. Who changed it

- **Joy:**
  - `TextRenderer` (#55), annotations and legends (#62 to #64, #70)
  - `Drawable`, the title and the axis labels (#89)
  - the ambiguity fix in `Legend::draw` (#94)
  - tick labels (#98) and the pipeline restructure (#100)
  - label fixes, the UML and the design decisions (#112)
- **Oscar:**
  - fixed build errors after the Week 3 merges (#71)
  - made tick labels use log ticks on a log axis (#106)
  - moved the labels below the new tick marks (#122)
- **End-of-project fixes, done with Claude Code:**
  - tick labels use `formatTickLabel`, so close ticks never print the same text
  - a NaN or infinite font size is refused (`docs/bug-report.md`)
- **This guide** was written by Member 4 with Claude Code, from Joy's `docs/text.md` and her code.
