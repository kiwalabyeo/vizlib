# WEEK 4 PROGRESS REPORT

**Project Title:** Design and Implementation of a Data Visualisation Library in C++

**Name:** Mugabe Destiny  
**Registration Number:** 25/U/08748/PS  
**Role:** Member 5 – Axis Scaling and Layout  

## 1. Work Done During the Week

During this week, I worked on improving the axis scaling functionality in the data visualisation library. I added logarithmic scaling to the original scale implementation, allowing the library to represent data using both linear and logarithmic scales.
I modified the scaling code to support logarithmic calculations using the base-10 logarithm. I also added validation to handle invalid values because logarithmic scaling cannot work with zero or negative values.
I further worked on tick generation. I added code to generate suitable tick values along the axes so that the numbers displayed on a graph are clear and easy to read. For linear scaling, I added a method that selects readable intervals such as 1, 2, 5, and 10 multiplied by powers of ten. For logarithmic scaling, I added a method that generates ticks using powers of ten.

## 2. Code and Files Updated

I worked on the following files:

- **scale.hpp and scale.cpp:** I added and improved logarithmic scaling functionality alongside the existing linear scaling.
- **ticks.hpp and ticks.cpp:** I added the `generateTicks()` and `generateLogTicks()` functions for generating tick values on linear and logarithmic axes.
- **axes.hpp and axes.cpp:** I worked on connecting the scaling functionality to the Axes class. I used `setXScale()` and `setYScale()` to allow the x-axis and y-axis to use different scaling types.

I also worked with the existing `dataToPixel()` and `pixelToData()` functions to support the conversion of data coordinates to screen coordinates and back.

## 3. Testing

I tested the logarithmic scaling functionality to check whether the calculations were working correctly. The test ran successfully and displayed the message:

`All logarithmic scaling tests passed!`

I also compiled and ran the tick generation test to check whether the tick generation code was working.

## 4. Challenges Faced

One of the challenges was understanding how logarithmic scaling works and how to include it in the existing linear scaling implementation. I also had to consider how to handle zero and negative values because they are not valid in logarithmic scaling.

Another challenge was generating tick values that are properly spaced and easy to read instead of displaying awkward decimal values.

## 5. Plan for the Next Week

Next week, I plan to continue improving the axis scaling and layout functionality. I will work on testing the axes and tick generation with the other parts of the library, fixing any errors found during integration, and improving the overall implementation.

## 6. Conclusion

During this week, I added logarithmic scaling and tick generation functionality to the original code. I also improved the handling of invalid logarithmic values and tested the new functionality. This work has helped improve the axis scaling component of the data visualisation library.
