# Week 3 Final Report: Object-Oriented Design in vizlib

## 1. Overview

This section of the report explains how three ideas of object-oriented programming (OOP) are used in vizlib, our C++ plotting library: encapsulation, inheritance and polymorphism. For each idea I point to real classes in the code and show them in hand-drawn class layouts, which are uploaded with this report. Each drawing shows the class name, the member data, the member functions and the access specifiers: `+` for public, `-` for private and `#` for protected.

## 2. Encapsulation

Encapsulation means that a class keeps its data private and lets other code reach it only through public functions. This protects the data from being changed in ways the class does not expect.

**Canvas.** The canvas stores its size and pixels in private members (`width_`, `height_` and `pixels_`). Other code uses public functions such as `width()`, `height()`, `set_pixel()`, `get_pixel()` and the drawing functions (`draw_line()`, `draw_rect()`, `fill_rect()`, `draw_circle()` and `fill_circle()`). The public functions protect the data. For example, `set_pixel()` ignores coordinates outside the picture, and `get_pixel()` reports an error for invalid coordinates.

**Legend.** The legend keeps its entries (`entries_`) and its position (`position_`) private. Other code adds entries with `add_entry()`, reads them with `get_entries()`, empties the legend with `clear()`, and moves it with `set_position()` and `get_position()`.

**TextRenderer.** The text renderer hides how fonts are handled. Its only data member is a private pointer (`impl_`) to a hidden `Impl` structure defined inside `text.cpp`. Users of the class only see the public functions `measure_text()` and `draw_text()`, so the font code can change without affecting other code. Copying a `TextRenderer` is disabled, but it can be moved.

**summaryStatistics.** The statistics class keeps the original data (`Data`) and a sorted copy (`Sorted_data`) private. The constructor sorts the copy once, so the public functions (`mean()`, `median()`, `range()`, `mode()`, `standard_deviation()`, `quantile()`, `interquartile_range()`, `count()` and `histogram_bin_edges()`) can always rely on sorted data. These functions are marked `const`, which means they do not change the stored data. Only `collect_data()` replaces the data, and it sorts the new copy again.

**ScatterPlot.** The constructor of `ScatterPlot` checks its input and throws `std::invalid_argument` if the x and y lists have different lengths, are empty, or contain no point without a NaN. Because the data is private, a `ScatterPlot` that exists can always be drawn.

## 3. Inheritance

Inheritance means that one class builds on another. The derived (child) class receives the interface of the base (parent) class and adds or fills in its own behaviour.

In vizlib, `Plot` (in `plot.hpp`) is the base class for everything that draws data onto an `Axes`. It is an abstract class: it has no member data, a virtual destructor, and two pure virtual functions:

- `virtual DataBounds bounds() const = 0;` gives the data range the plot needs.
- `virtual void draw(Canvas& canvas, const Axes& axes) const = 0;` draws the plot.

`ScatterPlot` inherits from `Plot` with `class ScatterPlot : public Plot`. It adds its own private data (`x_`, `y_`, `style_` and `bounds_`) and provides the real code for `bounds()` and `draw()`, marked `override`. The histogram and KDE plot classes (in `histogram.hpp` and `kde_plot.hpp`) inherit from `Plot` in the same way. Because `Plot` has pure virtual functions, it cannot be created on its own.

The arrow in the drawing goes from the derived class to the base class: from `ScatterPlot` up to `Plot`.

## 4. Polymorphism

Polymorphism means that the same call can behave differently depending on the actual type of the object. In vizlib it works through the virtual functions of `Plot`.

The joint plot only knows about `Plot`. In `joint_plot.cpp`, each panel is told to draw itself without the joint plot knowing whether the panel is a scatter plot, a histogram or a KDE curve. The right `draw()` runs for each kind, and the right `bounds()` is used to share a scale between panels. This is why any kind of plot can be used in any panel, and why a new kind of plot (such as an ECDF plot) can be added later without changing the joint plot.

## 5. Access specifiers

| Class | Private | Public |
|---|---|---|
| `Plot` | none | virtual destructor, `bounds()`, `draw()` |
| `ScatterPlot` | `x_`, `y_`, `style_`, `bounds_` | constructor, `bounds()`, `draw()` |
| `Canvas` | `width_`, `height_`, `pixels_` | constructor, `width()`, `height()`, `set_pixel()`, `get_pixel()`, drawing functions |
| `Legend` | `entries_`, `position_` | `add_entry()`, `get_entries()`, `clear()`, `set_position()`, `get_position()` |
| `TextRenderer` | `impl_` | constructor, `measure_text()`, `draw_text()` |
| `summaryStatistics` | `Data`, `Sorted_data` | constructor, `collect_data()`, statistics functions |

Small structures such as `DataBounds` and `ScatterStyle` use public members, because they only hold a few values together. No `protected` members were found in the library, so none appear in the drawings.

## 6. Class layout drawings

The drawings are photographed and uploaded with this report:

1. **Figure 1: inheritance and polymorphism.** The abstract class `Plot` and the derived class `ScatterPlot`, with an arrow from `ScatterPlot` to `Plot`.
2. **Figure 2: encapsulation.** The classes `Canvas`, `Legend` and `TextRenderer`, each with private data and public functions.
3. **Figure 3: encapsulation.** The class `summaryStatistics`.

Each box shows the class name at the top, the member data in the middle and the member functions at the bottom.

## 7. Summary

- **Encapsulation** is used throughout the library: classes keep their data private and offer public functions that protect it.
- **Inheritance** is used in the plot classes, which all build on the abstract base class `Plot`.
- **Polymorphism** lets the joint plot draw any kind of plot through the shared `Plot` interface.
- Only `public` and `private` are used. There are no `protected` members, so the library does not yet need them.
