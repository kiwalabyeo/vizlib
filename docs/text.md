
# Text Rendering, Annotations, and Legends

## Overview

Vizlib provides features for measuring and drawing text, storing text annotations, and managing legend entries.

## Text Rendering

The `TextRenderer` class is used to measure text and draw it on a canvas using a font file.

Include the header:

```cpp
#include "vizlib/text.hpp"
```

Create a text renderer by providing the path to a font file:

```cpp
vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");
```

Measure text before drawing it:

```cpp
vizlib::TextMetrics metrics = text.measure_text("Hello", 16.0);
```

Draw text on a canvas:

```cpp
text.draw_text(canvas, "Hello", vizlib::Point{10.0, 20.0},
               16.0, vizlib::Color{0, 0, 0});
```

The font size must be positive for meaningful measurements and rendering.

## Text Annotations

The `Annotations` class stores text annotations with their positions, font sizes, and colors.

Include the header:

```cpp
#include "vizlib/annotations.hpp"
```

Add an annotation:

```cpp
vizlib::Annotations annotations;
annotations.add_text("Important point",
                     vizlib::Point{50.0, 60.0},
                     14.0,
                     vizlib::Color{255, 0, 0});
```

Retrieve stored annotations with `get_annotations()` and remove all annotations with `clear()`.

## Legends

The `Legend` class manages legend entries. Each entry contains a label and a color.

Include the header:

```cpp
#include "vizlib/legend.hpp"
```

Add entries and set the legend position:

```cpp
vizlib::Legend legend;
legend.add_entry("Series A", vizlib::Color{255, 0, 0});
legend.add_entry("Series B", vizlib::Color{0, 0, 255});
legend.set_position(vizlib::Point{10.0, 10.0});
```

Use `get_entries()` to retrieve the entries, `get_position()` to retrieve the position, and `clear()` to remove all entries.

## Testing

Unit tests for text rendering, annotations, and legends are included in the test suite. The tests are registered through `tests/CMakeLists.txt`.
