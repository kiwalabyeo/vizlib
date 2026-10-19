# Week 3 Progress Report
## Completed
* Implemented the core ECDF coordinate calculation logic in ecdf.cpp based on the design in ecdf.hpp.
* Added logic to safely handle empty vectors ie usingdata.empty() that returns an empty coordinate vector.
* Implemented data sorting using std::sort and empirical cumulative probability calculation (y = (i + 1) / N) with explicit type casting using static_cast<double>.
## In Progress
* Writing unit tests in tests/test_ecdf_plot.cpp to verify output coordinates for standard data distributions, unsorted inputs, duplicate values, and empty vectors.
## Challenges/Blockers
* Encountered minor C++ syntax and type-casting issues when converting vector indices ie std::size_t to floating-point values (double) during probability calculation, which were resolved.
## Next Week
* Finalize and execute unit tests for the ECDF plot module.
* Combine the ECDF coordinate generator with the Axis Scaling and SVG Plot Rendering modules.
* Add example usage code under examples/ demonstrating ECDF coordinate generation.
## AI Use
Tool: Gemini
Purpose: Debugging C++ syntax errors (type mismatches, missing braces, namespace scope), understanding std::sort usage, and learning safe numeric casting using static_cast<double>.
Reason: To proper implementation of code that adheres to standard C++ library practices.
