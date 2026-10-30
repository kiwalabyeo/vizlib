# Text Rendering, Annotations, Legends, and Labels

## Overview

Vizlib provides features for measuring and drawing text, storing text annotations, managing legend entries, and displaying figure titles, axis labels, and tick labels.

These features make charts easier to understand and interpret.

## Text Rendering

The `TextRenderer` class is used to measure text and draw it on a canvas using a font file.

### Usage

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

The `measure_text()` function returns the width and height of the text. The `draw_text()` function draws the text at the specified position.

The font size must be positive for meaningful measurements and rendering.

## Text Annotations

The `Annotations` class stores text annotations with their positions, font sizes, and colors.

Annotations can be used to highlight important points or provide additional information on a chart.

### Usage

Include the annotations header:

```cpp
#include "vizlib/annotations.hpp"
```

Create an annotations object and add an annotation:

```cpp
vizlib::Annotations annotations;

annotations.add_text("Important point",
                     vizlib::Point{50.0, 60.0},
                     14.0,
                     vizlib::Color{255, 0, 0});
```

Retrieve stored annotations with `get_annotations()` and remove all annotations with `clear()`.

### Drawing Annotations

Annotations can be drawn on a canvas using a `TextRenderer`:

```cpp
annotations.draw(canvas, text);
```

The drawing function uses the canvas and text renderer to display the stored annotations.

## Legends

The `Legend` class manages legend entries. Each entry contains a label and a color representing a data series.

Legends help users identify the different data series displayed on a chart.

### Usage

Include the legend header:

```cpp
#include "vizlib/legend.hpp"
```

Create a legend, add entries, and set its position:

```cpp
vizlib::Legend legend;

legend.add_entry("Series A", vizlib::Color{255, 0, 0});
legend.add_entry("Series B", vizlib::Color{0, 0, 255});

legend.set_position(vizlib::Point{10.0, 10.0});
```

Use `get_entries()` to retrieve the entries, `get_position()` to retrieve the legend position, and `clear()` to remove all entries.

### Drawing a Legend

Draw the legend on a canvas using a `TextRenderer`:

```cpp
legend.draw(canvas, text);
```

The legend displays each entry using its label and corresponding color.

## Figure Titles and Axis Labels

The `Figure` class supports figure titles and X-axis and Y-axis labels.

These labels provide information about the chart and the data being displayed.

### Usage

Include the figure header:

```cpp
#include "vizlib/figure.hpp"
```

Create a figure:

```cpp
vizlib::Figure figure;
```

Set the figure title and axis labels:

```cpp
figure.setTitle("Temperature Measurements");
figure.setXLabel("Time (seconds)");
figure.setYLabel("Temperature (Celsius)");
```

The labels can also be retrieved using:

```cpp
figure.getTitle();
figure.getXLabel();
figure.getYLabel();
```

### Drawing Figure Labels

Use `drawLabels()` to draw the figure title and axis labels on a canvas:

```cpp
figure.drawLabels(canvas, text);
```

The method requires a `Canvas` and a `TextRenderer`.

The figure title is displayed above the chart, while the X-axis and Y-axis labels identify the corresponding axes.

**Current limitation:** Y-axis title rotation is not currently supported. Therefore, the Y-axis label is drawn horizontally.

## Tick Labels

Tick labels are numerical values displayed along the X and Y axes.

They help users identify the values represented by different positions on a graph.

### Generating Tick Values

Include the ticks header:

```cpp
#include "vizlib/ticks.hpp"
```

Use `generateTicks()` to generate numerical tick values within a specified range:

```cpp
std::vector<double> ticks =
    vizlib::generateTicks(0.0, 100.0, 5);
```

The function accepts three arguments:

- `min` – The minimum value of the range.
- `max` – The maximum value of the range.
- `targetCount` – The desired number of tick values. It defaults to 5 if omitted.

For example, the function can generate tick values for a graph with a range from 0 to 100.

### Drawing Tick Labels

The `Axes` class provides `drawTickLabels()` to display numerical tick labels along the X and Y axes.

Include the axes header:

```cpp
#include "vizlib/axes.hpp"
```

Draw the tick labels using a canvas and a text renderer:

```cpp
axes.drawTickLabels(canvas, text);
```

The method generates tick values from the axes data range and uses the `TextRenderer` to display them.

Tick labels make graphs easier to read by showing the numerical scale of each axis.

## Testing

Unit tests for text rendering, annotations, and legends are included in the test suite. The tests are registered through `tests/CMakeLists.txt`.

The tests cover:

- Text measurement and rendering.
- Adding, retrieving, and drawing text annotations.
- Adding, retrieving, and drawing legend entries.
- Setting and retrieving figure titles and axis labels.
- Drawing figure titles and axis labels.
- Generating numerical tick values.
- Drawing tick labels along the axes.

These tests help verify that the text rendering components work correctly and integrate with the rest of the Vizlib library.
