# WEEK 5 PROGRESS REPORT

**Name:** Mugabe Destiny  
**Registration Number:** 25/U/08748/PS  
**Project:** Design and Implementation of a Data Visualisation Library in C++  
**Role:** Member 5 — Axis Scaling and Layout  
**Reporting Period:** Week 5 (October 26–31, 2026)

## 1. Work Completed

During Week 5, I focused on completing and improving the Axis Scaling and Layout module. I worked on integrating the axis functionality with the shared rendering components and improving how axes are displayed.

The main activities included:

- Completed the implementation of linear and logarithmic axis scaling.
- Improved data-to-pixel and pixel-to-data coordinate conversions.
- Worked on generating readable tick positions and formatting tick labels.
- Implemented drawing of horizontal and vertical axis lines and tick marks using the shared Canvas and Color components.
- Improved validation for invalid ranges and logarithmic values.
- Reviewed the `Figure` and `Axes` layout functionality, including margins and plotting areas.
- Worked on testing and resolving header and source-file integration issues.

## 2. Current Status

The main functionality of the Axis Scaling and Layout module has been implemented. The module supports linear and logarithmic scaling, coordinate conversion, tick generation, axis drawing and basic figure layout.

I also reviewed how other plotting components use the axis conversion functions to position their data correctly.

## 3. Challenges Encountered

- Resolving header and source-file declaration issues during compilation.
- Ensuring logarithmic scaling handles invalid values correctly.
- Maintaining consistency between data coordinates and pixel positions.
- Integrating axis drawing with the shared Canvas and Color components.

## 4. Dependencies

- **Member 4 — Kiwalabye Oscar Muwanguzi:** Used the axis coordinate conversion functionality for positioning scatter plots and joint plot elements.
- **Member 6 — Nambalirwa Flavia Cate:** Provided shared Canvas and Color components used in drawing axis lines and tick marks.
