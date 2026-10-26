# WEEK 3 PROGRESS REPORT

**Project Title:** Design and Implementation of a Data Visualisation Library in C++

**Name:** MUGABE DESTINY 
**Registration Number:** 25/U/08748/PS  
**Role:** Member 5 — Axis Scaling and Layout  

## 1. Introduction

During Week 3, I continued working on the axis scaling and layout part of the data visualisation library. 
My main task this week was to develop a function for generating suitable tick values for graph axes. This work builds on the basic scaling and layout functionality I developed in Week 2.

This report explains the work completed during Week 3, how I applied object-oriented programming concepts in my code,
the challenges I faced, and how I used artificial intelligence during development.

## 2. Work Completed During Week 3

During Week 3, I created `ticks.hpp` and `ticks.cpp` to handle the generation of tick values for graph axes.

I implemented the `generateTicks()` function, which takes the minimum value, maximum value, and target number of ticks as inputs. It calculates a suitable interval and generates readable tick values for the given range.

The function uses mathematical operations such as `std::floor()`, `std::ceil()`, `std::pow()`, and `std::log10()` to calculate appropriate intervals. It selects convenient steps based on values such as 1, 2, 5, and 10 multiplied by powers of ten.

I also added input validation to handle invalid ranges and target tick counts. The function throws an `std::invalid_argument` exception when the inputs are invalid.

The following code shows part of the validation and calculation:

```cpp
if (!std::isfinite(min) || !std::isfinite(max) ||
    min >= max || targetCount < 2) {
    throw std::invalid_argument("Invalid tick range");
}

double rawStep = (max - min) / (targetCount - 1);
double magnitude =
    std::pow(10.0, std::floor(std::log10(rawStep)));
```

This work helps the library generate clear and readable tick values instead of using inconvenient intervals.

## 3. Application of Object-Oriented Programming Concepts

Although my main task during Week 3 involved a standalone tick-generation function, I also applied object-oriented programming concepts in the `Figure`, `Axes`, and `LinearScale` classes developed during Week 2.

### 3.1 Encapsulation

I applied my knowledge of encapsulation in the `Figure`, `Axes`, and `LinearScale` classes by keeping their internal data members private and providing public member functions to access or modify the data and perform operations.

In `include/vizlib/figure.hpp`, the `Figure` class contains private data members such as `width_`, `height_`, `leftMargin_`, `topMargin_`, `rightMargin_`, and `bottomMargin_`. Its public functions include `setSize()`, `setMargins()`, `getWidth()`, and `getHeight()`.

The following code from `src/figure.cpp` shows how the `setSize()` function validates the figure dimensions before updating them:

```cpp
void Figure::setSize(int width, int height) {
    if (width <= 0 || height <= 0 ||
        leftMargin_ + rightMargin_ >= width ||
        topMargin_ + bottomMargin_ >= height) {
        throw std::invalid_argument("Invalid figure size");
    }

    width_ = width;
    height_ = height;
}
```

In `include/vizlib/axes.hpp`, the `Axes` class keeps its data ranges, plot area, and scaling objects private. Public functions such as `setDataRange()`, `setPlotArea()`, `dataToPixel()`, and `pixelToData()` provide the operations needed to manage the axes and map coordinates.

Similarly, in `include/vizlib/scale.hpp`, the `LinearScale` class keeps its range variables private and provides public functions such as `setRange()`, `dataToPixel()`, and `pixelToData()`.

These implementations demonstrate how I used encapsulation to organise the classes and control how their data is accessed and modified.

### 3.2 Polymorphism

I applied my knowledge of compile-time polymorphism through function overloading in the `Figure` class. 
This was part of my earlier work on figure layout.

In `include/vizlib/figure.hpp`, I declared two functions with the same name but different parameters:

```cpp
Axes& addAxes();
Axes& addAxes(Rect area);
```

The first function adds an axes area using the default layout, while the second accepts a `Rect` object specifying the area to use.

In `src/figure.cpp`, the first function calls the second function with a rectangle based on the figure's dimensions:

```cpp
Axes& Figure::addAxes() {
    return addAxes(Rect{
        0.0, 0.0,
        static_cast<double>(width_),
        static_cast<double>(height_)
    });
}
```

This demonstrates function overloading because the compiler selects the appropriate `addAxes()` function depending on the arguments supplied.

### 3.3 Inheritance

I did not use inheritance in the classes developed for my part of the project during the period covered by this report.
The `Figure`, `Axes`, and scaling classes were developed without deriving them from other classes.

## 4. Challenges Faced

One challenge was choosing suitable intervals for the tick values so that they would be easy to read on a graph. 
I addressed this by using mathematical calculations to select convenient intervals based on values such as 1, 2, 5, and 10.

Another challenge was understanding how to organise declarations in header files and implementations in source files. 
I worked on keeping the function declarations in `ticks.hpp` and the function implementation in `ticks.cpp`.

I also included input validation to prevent the function from processing invalid ranges or an unsuitable target tick count. 
Further testing and integration with the other project modules are still necessary.

## 5. Use of AI

I used ai to help me understand the organisation of header and source files, the mathematical functions used in tick generation, and the application of object-oriented programming concepts in my classes.

The explanations helped me understand how the functions work and how encapsulation and function overloading are applied in my implementation. 
I used this assistance to support my understanding and development of the project.

## 6. Conclusion and Next Steps

During Week 3, I created `ticks.hpp` and `ticks.cpp` and implemented the `generateTicks()` function to produce suitable tick values for graph axes. 
I also added input validation to handle invalid inputs.

My earlier work on the `Figure`, `Axes`, and `LinearScale` classes demonstrates encapsulation, while the overloaded `Figure::addAxes()` functions demonstrate compile-time polymorphism. 
I did not use inheritance in the classes developed for my part during this period.

My next steps are to integrate logarithmic scaling into the axes, improve the handling of invalid data values, support multiple axes for joint plots, test the implementation with other group members' modules, fix integration errors, and complete the necessary documentation.
