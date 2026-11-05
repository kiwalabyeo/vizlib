# Axis Scaling and Layout: Linear and Logarithmic Scales, Ticks and Figure Layout

- **Owner:** Mugabe Destiny (Member 5)
- **Module:** Axis Scaling and Layout
- **Headers:** `include/vizlib/scale.hpp`, `axes.hpp`, `figure.hpp`, `ticks.hpp`
- **Sources:** `src/scale.cpp`, `src/axes.cpp`, `src/figure.cpp`, `src/ticks.cpp`
- **Tests:** `tests/test_axis.cpp`, `tests/test_log_scale.cpp`, `tests/test_ticks.cpp`
- **Related components:** Canvas, Color and TextRenderer

## 1. What it does

This module controls how data values are positioned and displayed on a graph. It converts data coordinates into pixel positions, manages the X and Y axes, generates readable tick marks and labels, and controls the overall plotting layout.

The main components are:

- **`LinearScale`:** converts data values into pixel positions using a linear relationship. Equal changes in data values produce equal changes in position.
- **`LogScale`:** converts data values using logarithmic scaling. It is useful when values cover a wide range of magnitudes.
- **`Axes`:** manages the X and Y axes, data ranges, plotting area and coordinate conversions.
- **`Figure`:** manages the overall figure dimensions, margins and arrangement of one or more plotting areas.
- **Tick generation:** calculates suitable positions for tick marks and produces readable tick labels.
- **Axis drawing:** draws horizontal and vertical axis lines and their tick marks using the shared Canvas component.

## 2. Files and how they work

### Header files

**`scale.hpp`**

Declares the scaling classes used to convert data values into screen positions.

- `LinearScale` handles linear scaling.
- `LogScale` handles logarithmic scaling.
- `setRange()` sets the data and pixel ranges used by a scale.
- `dataToPixel()` converts a data value into a pixel position.
- `pixelToData()` converts a pixel position back into a data value.

This header provides the interface for the scaling functionality implemented in `scale.cpp`.

**`axes.hpp`**

Declares the `Axes` class, which manages the coordinate system and plotting area of a graph.

Its main functions include:

- `setDataRange()` sets the minimum and maximum data values.
- `setPlotArea()` defines the plotting area's position and dimensions.
- `setXScale()` selects the scaling type for the X-axis.
- `setYScale()` selects the scaling type for the Y-axis.
- `dataToPixel()` converts data coordinates into pixel coordinates.
- `pixelToData()` converts pixel coordinates back into data values.
- `getPlotArea()` retrieves the plotting area.
- `drawAxes()` draws axis lines and tick marks.
- `drawTickLabels()` displays the labels associated with the axes.

The `Axes` class connects the scaling functionality with the actual graph layout.

**`figure.hpp`**

Declares the `Figure` class, which manages the overall graph or figure.

It provides functionality for:

- Managing the figure's width and height.
- Setting margins around the plotting area.
- Adding one or more `Axes` objects.
- Providing access to the axes contained in the figure.
- Managing figure titles and axis labels.
- Organising the available plotting space.

The `Figure` class makes it possible to organise graph elements within one figure, including layouts with multiple plotting areas.

**`ticks.hpp`**

Declares the functions responsible for generating and formatting axis ticks.

- `generateTicks()` generates suitable tick positions for linear scales.
- `generateLogTicks()` generates tick positions for logarithmic scales.
- `formatTickLabel()` converts tick values into readable text labels.

These declarations allow the axes module to obtain tick positions and labels without implementing the tick-generation algorithms inside `axes.cpp`.

### Source files

**`scale.cpp`**

Implements the linear and logarithmic scaling operations.

Its responsibilities include:

- Calculating the relationship between data ranges and pixel ranges.
- Converting data values into pixel coordinates.
- Converting pixel coordinates back into data values.
- Handling linear and logarithmic transformations.
- Validating ranges and rejecting invalid logarithmic values.

For logarithmic scaling, data values must be positive because zero and negative values cannot be used in logarithm calculations.

**`axes.cpp`**

Implements the functionality of the `Axes` class.

It is responsible for:

- Initialising the X and Y scales.
- Setting and validating data ranges.
- Setting and updating the plotting area.
- Selecting the scaling type for each axis.
- Performing coordinate conversions through the active scale.
- Generating and displaying tick labels.
- Drawing horizontal and vertical axis lines.
- Drawing tick marks using Canvas drawing functions.

It connects the scaling and tick-generation components with the graph's visible axes.

**`figure.cpp`**

Implements the `Figure` class.

It manages the overall graph layout by:

- Initialising figure dimensions.
- Setting the figure size.
- Setting margins.
- Adding axes to the figure.
- Managing access to the axes.
- Managing figure titles and axis labels.
- Providing the available drawing space for graph elements.

It helps ensure that plotting areas are arranged within the figure while leaving space for margins and labels.

**`ticks.cpp`**

Implements the tick-generation and formatting functions declared in `ticks.hpp`.

Its responsibilities include:

- Calculating suitable tick intervals for linear axes.
- Generating tick values within a specified data range.
- Generating logarithmic tick positions.
- Formatting tick values into readable labels.
- Avoiding unnecessarily complicated tick intervals.

For linear scales, the tick-generation algorithm aims to produce convenient intervals such as 1, 2, 5 or 10 instead of awkward intervals such as 4.37.

For logarithmic scales, tick positions are generated using powers of ten where appropriate.

### Test files

**`test_axis.cpp`**

Tests the main axis functionality.

It checks:

- Setting data ranges.
- Setting plotting areas.
- Converting data values into pixel positions.
- Converting pixel positions back into data values.
- Basic figure and axes operations.
- Coordinate conversion using configured plotting areas.

**`test_log_scale.cpp`**

Tests logarithmic scaling functionality.

It checks:

- Setting logarithmic ranges.
- Converting positive data values into pixel positions.
- Converting pixel positions back into data values.
- Handling invalid logarithmic values such as zero and negative numbers.

**`test_ticks.cpp`**

Tests tick generation and formatting.

It checks:

- Generation of linear tick values.
- Generation of logarithmic tick values.
- Whether generated ticks fall within the required range.
- Formatting tick values into readable labels.
- Handling invalid ranges and tick-generation parameters.

## 3. How it works

**Data-to-pixel conversion.**

Graphs are drawn using pixel positions, while the original datasets contain data values. The scaling classes provide the conversion between these two coordinate systems.

For example, consider a horizontal axis with a data range of 0 to 100 and a pixel range of 0 to 500.

A data value of 50 is positioned at pixel 250. This allows plotting modules to use the same coordinate conversion functionality rather than calculating pixel positions independently.

**Linear scaling.**

Linear scaling maps values proportionally across the available plotting area. It is suitable for data where equal numerical differences should have equal visual distances.

**Logarithmic scaling.**

Logarithmic scaling uses logarithmic transformations to represent data that covers a wide range of magnitudes. The input range must remain positive.

**Axes and plotting area.**

The `Axes` class maintains the X and Y data ranges and the pixel dimensions of the plotting area. It uses the selected scale for each axis when converting coordinates.

**Tick generation.**

The tick functions determine suitable values to display along the axes. These values are then formatted into text and displayed through the text-rendering component.

**Figure layout.**

The `Figure` class manages the available drawing space, including margins and multiple axes. This helps plotting modules organise their graphs within the same figure.

**Drawing.**

The `drawAxes()` function uses Canvas drawing operations to draw the axis lines and tick marks. The `drawTickLabels()` function works with tick generation and TextRenderer to display readable labels.

## 4. Why it is written this way

- **Separation of responsibilities.** Scaling, axes, figure layout and tick generation are implemented separately. This makes the code easier to maintain and test.
- **Different scaling methods.** Separate `LinearScale` and `LogScale` classes allow the library to support different ways of displaying data without placing all scaling calculations inside the `Axes` class.
- **Reusable coordinate conversion.** Plotting modules can use `dataToPixel()` and `pixelToData()` without implementing their own conversion formulas. This allows different plots to use a consistent coordinate system.
- **Encapsulation.** The `Axes` and `Figure` classes manage their own data and provide functions for changing ranges, dimensions and layout.
- **Validation.** Range and layout functions check their inputs to prevent invalid configurations. Logarithmic scaling rejects zero and negative values.
- **Reusable tick generation.** `generateTicks()` and `generateLogTicks()` are separate from drawing functions, allowing tick calculations to be tested independently.
- **Shared rendering components.** Canvas and Color provide the drawing operations and appearance used by axis lines and tick marks, while TextRenderer handles text display.

## 5. How it is tested

The module uses separate test files for axis operations, logarithmic scaling and tick generation.

- **Linear scaling:**
  - Data values are converted into expected pixel positions.
  - Pixel positions can be converted back into data values.
  - X and Y coordinate conversions are checked.

- **Logarithmic scaling:**
  - Positive data ranges are accepted.
  - Logarithmic coordinate conversions are checked.
  - Zero and negative values are rejected.

- **Tick generation:**
  - Linear tick positions are generated within the requested range.
  - Logarithmic tick positions are generated for positive ranges.
  - Tick values are formatted into readable labels.
  - Invalid ranges and parameters are checked.

- **Figure and axes layout:**
  - Figure dimensions and margins are validated.
  - Axes can be added to a figure.
  - Plotting areas are checked.
  - Coordinate conversion is tested using the configured plotting area.

## 6. Limitations

- Logarithmic scaling only supports positive data values.
- Tick spacing depends on the tick-generation algorithm and selected data range.
- Tick labels require the TextRenderer component and an available font.
- Drawing axis lines and tick marks depends on the shared Canvas and Color components.
- The Figure class manages the layout, while individual plotting modules are responsible for drawing their actual data.
- When using logarithmic axes, the selected data range and any padding must remain positive.

## 7. Who used it and whose files I used

- **Member 4 — Kiwalabye Oscar Muwanguzi:** Used my `Axes` coordinate conversion functionality, particularly `axes.dataToPixel()`, to position scatter plot and joint plot elements. His joint plot also uses shared X and Y axis ranges to align the centre and marginal panels.

- **Member 6 — Namalirwa Flavia Cate:** I used her shared `canvas.hpp` and `color.hpp` components when implementing axis lines and tick marks. Canvas provided the drawing operations, while Color provided the colour information used for rendering.

## 8. Development History

**Week 1 — Research and Planning**

Reviewed axis scaling and graph layout requirements and identified the main responsibilities of the module.

**Week 2 — Linear Scaling and Basic Layout**

Worked on linear scaling, coordinate conversion and the basic `Figure` and `Axes` structures. Established the initial relationship between data ranges and pixel positions.

**Week 3 — Tick Generation**

Worked on generating readable tick positions and formatting tick labels for graph axes. Developed the functionality for linear tick generation and its integration with the axes.

**Week 4 — Logarithmic Scaling**

Worked on logarithmic scaling, validation of invalid values and testing the scaling operations. Improved the handling of positive ranges and logarithmic coordinate conversion.

**Week 5 — Axis Drawing, Testing and Integration**

Worked on drawing axis lines and tick marks, reviewing the scaling and tick-generation functionality, and integrating the module with shared rendering components. Also worked on resolving issues involving function declarations and source/header integration.

## 9. Related Project Components

- **Canvas:** provides drawing operations used to draw axis lines and tick marks.
- **Color:** provides colour information for graph elements.
- **TextRenderer:** handles the display of tick labels, titles and other text.
- **Plotting modules:** use the axes' coordinate conversion functions to position their data correctly.
- **Image Export:** works with the rendered figure when the graph is saved as an image.
