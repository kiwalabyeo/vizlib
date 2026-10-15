#  MUGABE DESTINY

**Reg. No:** 25/U/08748/PS

**Role:** Axis Scaling and Layout

## Week 2 Progress Report (October 5–11, 2026)

### 1. Work Done and In Progress

This week, I am working on the basic linear scale, reverse mapping, and starting the `Figure` and `Axes` classes.

My work includes:

- **Linear scaling:** Converting data values into pixel positions so that points appear correctly on the graph.
- **Reverse mapping:** Converting pixel positions back into data values.
- **Figure class:** Starting a class to manage the size of the figure and the axes inside it.
- **Axes class:** Starting a class to manage the x-axis, y-axis, data ranges, and graph area.
- **Testing:** Checking whether the scaling functions give correct results.

### 2. Classes and Functions Used

I am working with the `Figure` and `Axes` classes.
I am also using the `dataToPixel()` function to convert data values into pixel positions and the `pixelToData()` function to convert pixel positions back into data values.

### 3. Header and Source Files

I am working on;
`scale.hpp` and `scale.cpp` for scaling and reverse mapping,
`figure.hpp` and `figure.cpp` for the figure
`axes.hpp` and `axes.cpp` for the graph area and axis ranges.
I will also work on `ticks.hpp` and `ticks.cpp` to generate suitable tick values.

### 4. Libraries and Tools

I will use `<cmath>` for mathematical operations, `<stdexcept>` for handling invalid inputs, and `<vector>` for storing multiple axes.
I will use Doctest to test my functions and CMake to build the project.

### 5. Information Needed from Other Members

I need to consult **Member 6, Namalirwa Flavia Cate**, for the actual `Canvas` and `Rect` definitions so that my graph area matches the drawing system.

I also need to consult **Member 4, Kiwalabye Oscar Muwanguzi**, to understand how multiple axes should be arranged for the joint plot.

### 6. Challenges
At this early stage, I have not faced any major challenges because I am still beginning my work.
I have not yet run my code to check whether it works correctly. 
My main focus is to make sure that the code is written correctly and to identify and fix any errors during testing.
Since this is a group project, I will also need to make sure that my code works well with the other members' work.

### 7. Expected Output for the Week

I aim to implement and test the linear scaling and reverse mapping functions and begin building the `Figure` and `Axes` classes.

### 8. AI use
I used chatgpt  to help me understand the purpose of the header files and source files, the classes and functions needed for beginning part ,
and how to organise the code for linear scaling, reverse mapping, and the Figure and Axes classes.
