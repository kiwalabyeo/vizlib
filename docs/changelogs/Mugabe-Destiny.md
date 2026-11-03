# Mugabe Destiny — Axis Scaling and Layout

**Registration Number:** 25/U/08748/PS  
**Project:** Design and Implementation of a Data Visualisation Library in C++  
**Role:** Member 5 — Axis Scaling and Layout

## Week 1: Research and Planning

**Work done:**
- Researched the purpose of the Data Visualisation Library.
- Studied the responsibilities of the Axis Scaling and Layout component.
- Identified the main features to implement, including linear scaling, coordinate conversion, tick generation and figure layout.

**Files added:** None.

**Functions added:** None.

## Week 2: Basic Scaling and Figure Layout

**Files added:**
- `include/vizlib/scale.hpp`
- `src/scale.cpp`
- `include/vizlib/axes.hpp`
- `src/axes.cpp`
- `include/vizlib/figure.hpp`
- `src/figure.cpp`

**Functions implemented:**

In `scale.cpp`:
- `LinearScale::LinearScale()`
- `LinearScale::setRange()`
- `LinearScale::dataToPixel()`
- `LinearScale::pixelToData()`

In `axes.cpp`:
- `Axes::Axes()`
- `Axes::setDataRange()`
- `Axes::setPlotArea()`
- `Axes::dataToPixel()`
- `Axes::pixelToData()`
- `Axes::getPlotArea()`

In `figure.cpp`:
- `Figure::Figure()`
- `Figure::setSize()`
- `Figure::setMargins()`
- `Figure::addAxes()`
- `Figure::getAxes()`
- `Figure::getAxesCount()`
- `Figure::getWidth()`
- `Figure::getHeight()`

**Testing:**
- Added tests for basic linear scaling, coordinate conversion and Axes functionality.

## Week 3: Tick Generation

**Files added:**
- `include/vizlib/ticks.hpp`
- `src/ticks.cpp`

**Functions implemented:**
- `generateTicks()` — generates readable tick values for linear scales.
- Integrated tick generation into the Axes component.
- `Axes::drawTickLabels()` — displays numerical tick labels on the axes.

**Testing:**
- Added tests for tick generation and axis functionality.

## Week 4: Logarithmic Scaling

**Files updated:**
- `include/vizlib/scale.hpp`
- `src/scale.cpp`
- `include/vizlib/axes.hpp`
- `src/axes.cpp`
- `include/vizlib/ticks.hpp`
- `src/ticks.cpp`

**Functions implemented:**

In `scale.cpp`:
- `LogScale::LogScale()`
- `LogScale::setRange()`
- `LogScale::dataToPixel()`
- `LogScale::pixelToData()`

In `axes.cpp`:
- `Axes::setXScale()`
- `Axes::setYScale()`
- Updated `Axes::dataToPixel()` to support logarithmic scaling.
- Updated `Axes::pixelToData()` to support logarithmic scaling.

In `ticks.cpp`:
- `generateLogTicks()` — generates tick values for logarithmic scales.

**Testing:**
- Added tests for logarithmic scaling, coordinate conversion and invalid logarithmic values.

## Week 5: Testing, Integration and Final Improvements

**Files updated:**
- `include/vizlib/axes.hpp`
- `src/axes.cpp`

**Functions added:**
- `Axes::drawAxes()` — draws horizontal and vertical axis lines and their tick marks.

**Other improvements:**
- Reviewed compatibility with the group's updated `Canvas` and `Color` components.
- Continued testing the Axis Scaling and Layout implementation.
- Worked on integrating axis lines and tick marks into plot rendering.

**Testing:**
- Reviewed existing axis, tick and logarithmic scaling tests.
- Worked on final integration and testing of the new axis drawing functionality.

