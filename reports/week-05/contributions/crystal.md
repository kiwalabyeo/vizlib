# WEEK 5 PROGRESS REPORT
Name: Sengendo Crystal Zawedde
Reg No: 25/U/08615/PS
Module: ECDF and Rug Plot Modules
## WHAT WAS DONE
After speaking with team members in charge of other modules, I found that my original code was missing required shared interfaces. I imported the necessary header files from vizlib, including axes, canvas, color, concepts, geometry, and plot.
I updated the code so that it now supports both ECDF plots and Rug plots.
I added template constructors using "template <Numeric T>" for both ECDFPlot and RugPlot. This uses C++20 concepts to allow users to pass vectors of any numeric type like int or float, automatically converting them to doubles using to_doubles(data) so the user does not have to convert numbers manually before plotting.
I added a helper function called without_nan to remove non-number NaN values before sorting data or placing points on an axis. (function is subject to change to include a without_nan function from the preprocessing.cpp
I added a padded_range helper function to add 5 percent padding around data ranges to prevent division by zero and stop plot lines from touching axis borders.
I updated ECDFPlot to inherit from the common Plot class in plot.hpp. It now includes custom styling, full drawing logic with step lines on a Canvas using Axes scaling, and error checking for invalid data (pending change to include functions from preprocessing.cpp.
I implemented the RugPlot class to draw vertical marks along the bottom edge of the plot area for every data value.
## NEXT STEPS
Reconcile all code changes to include data preprocessing before submission.
