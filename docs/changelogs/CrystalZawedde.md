#Changelog: Sengendo Crystal Zawedde
Module: ECDF plots
GitHub: @CrystalZawedde

##2026-10-22
Changed: Added include/viz/ecdf_plot.hpp containing the Point struct and ECDFPlot class declaration.
Why: To define a clean public C++ interface that separates function declarations from implementation code.
Verified syntax and inclusion using compiler header checks.
Checked class structure alignment within the viz namespace to ensure non-nested definitions.

##2026-10-28
Changed: Implemented compute_coordinates() in src/ecdf_plot.cpp with data.empty() safety checks, std::sort, and numeric casting (static_cast<double>).
Why: To calculate step-function Empirical Cumulative Distribution Function coordinates from raw input data.
Resolved IDE/compiler errors and namespace scope issues.
Manually checked calculation step logic for empty data inputs (returns {}) and non-empty vector sorting.

