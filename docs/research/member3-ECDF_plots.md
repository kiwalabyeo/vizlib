# Name: Sengendo Crystal Zawedde

# Reg. No.: 25/U/08615/PS

# Area of Coverage: Creating the ECDF Plot Module

## Introduction
An ECDF (Empirical Cumulative Distribution Function) plot shows what percentage of the total data is less than or equal to a particular value x.

For each observation x in a sequence, the ECDF calculates:

F(x) = Number of observations <= x / Total observations

## What the Module Does

My ECDF module will receive cleaned numerical data in the form of std::vector<double> from the Data Preprocessing and Statistical Summaries module.

It will also work with the Axis Scaling module, which will make sure the x-axis is scaled according to the minimum and maximum values in the dataset while the y-axis remains fixed from 0.0 to 1.0.

After generating the ECDF step coordinates, my module will pass them to the Plot Rendering module, which will use the coordinates to draw the ECDF.

The Labels and Annotations module will then add things like axis titles, tick marks, and legends, while the Image Export module will handle putting the final plot into a format such as SVG or PPM.

## ECDF Plot Algorithm

1. Start.

2. Receive a numerical dataset as std::vector<double>.

3. Check whether the dataset is empty.

4. If the dataset is empty, return an empty result or an appropriate error.

5. Determine the number of observations, n.

6. Sort the data in ascending order using std::sort().

7. Initialize an empty collection to store the ECDF step coordinates.

8. Set the initial cumulative probability to 0.0.

9. For each sorted data value x_i:

   * Calculate the cumulative probability:

     y_i = i/n

   * Create a horizontal step from the previous x-value to x_i at the previous cumulative probability.

   * Create a vertical step at x_i from the previous cumulative probability to y_i.

   * Store these coordinates as part of the ECDF path.

   * Update the previous cumulative probability to y_i.

10. After processing all observations, ensure the final cumulative probability is 1.0.

11. Return the generated step coordinates to the plotting module.

12. The rendering module uses these coordinates to draw the ECDF on axes where:

    * x-axis = data values
    * y-axis = cumulative probability from 0.0 to 1.0

13. End.

## Module Files

The module will be comprised of two files:

### ecdf.hpp

A header file containing the functions and data structures provided by the ECDF module.

### ecdf.cpp

The module implementation file.

This separation allows for the header file to be called beyond just the ECDF module while hiding the complexities of the code itself (encapsulation).

## Implementation Plan

### Week 1: Architecture and Interface

* Talk to the Plot Rendering and Axis Scaling members to agree on how the ECDF module will pass its generated coordinates to the other modules.
* Finalize the ecdf_plot.hpp header file and decide on the functions and data structures my module will provide.

### Week 2: Core ECDF Implementation

* Implement the main ECDF logic in ecdf_plot.cpp.
* Sort the input data and calculate the cumulative probabilities.
* Generate the horizontal and vertical step coordinates.
* Write unit tests for cases like empty data, one value, duplicate values, and normal datasets.

### Week 3: Integration and Rendering

* Connect my ECDF module to the Axis Scaling and Plot Rendering modules.
* Make sure the generated coordinates are correctly converted into the final plot.
* Create an example in examples/ecdf_demo.cpp to demonstrate that my module works.

### Week 4: Testing and Final Improvements

* Test the module with larger datasets and different types of input.
* Fix any bugs and improve the implementation where necessary.
* Add clear comments and documentation for my code.
* Update the project README.md with information about my ECDF module.
