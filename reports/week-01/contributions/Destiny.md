 MUGABE DESTINY 25/U/08748/PS

 AXIS SCALING AND LAYOUT

Role: Axis Scaling and Layout
Overview
My role is to work on how values are placed on the graph and how the graph is arranged. 
My work will help the other members place their plots and labels in the correct positions.

 The  main responsibilities include:
Creating linear and logarithmic scales for the graph.
Changing data values into positions on the screen.
Changing screen positions back into data values.
Handling invalid values such as zero and negative values when using a logarithmic scale.
Creating simple and easy-to-read tick values such as 0, 5, 10 instead of values like 0, 4.37, 8.74.
Creating the Figure and Axes classes.
Setting margins and the area where the graph will be drawn.
Allowing more than one Axes to be used in one Figure, especially for the joint plot.

 Implementation Tasks
I will create the scaling and layout parts of the project using the required .hpp and .cpp files.
The files include:
scale.hpp and scale.cpp
ticks.hpp and ticks.cpp
figure.hpp and figure.cpp
axes.hpp and axes.cpp

The scaling part will use the data values and the size of the graph to decide where each value should be placed.
For linear scaling, equal changes in the data will have equal spaces on the graph. For logarithmic scaling,
the values will be placed based on their powers.
The tick system will choose suitable spaces between the tick marks and create values that are easy to read.
The Figure will represent the whole graph image, while the Axes will represent the area where a graph is drawn. 
The layout will also allow more than one Axes to be placed in the same Figure.
I will also write tests to make sure the scaling, ticks, and Figure work correctly.

The tests will include:
test_scale.cpp
test_ticks.cpp
test_figure.cpp

libraries and tools
C++17: used to write the program.
doctest: used to test my code.
Canvas: used as the drawing area provided by Member 6.
CMake: used to build the project.
Git/GitHub: used to save and share the project code with the group.

*What others need from me
Members working on ECDF and rug plots will need my Axes and scaling parts to know where to place their plots.
Member 4, who is working on the joint plot, will need my support for using more than one Axes in a Figure.
Member 8, who is working on Labels and Annotations, will need the plot area and tick information to place titles, 
axis labels, and tick labels correctly.
Member 6 will need the layout information to know where drawings should be placed on the Canvas.

What I need from other members
Member 1: I need the data and data ranges from the data-processing part so that I can set the correct axis ranges.
Member 6: I need the Canvas information so that I know the size and position where the graph will be drawn.
Members 3 and 4: I need to know how their plots will use the Axes so that I can make the scaling and layout work correctly.
Member 8: I need to coordinate on tick labels, titles, and axis labels so that they are placed in the correct positions.
Member 7: I need to make sure my files work correctly with the final build and image-saving part.

Error handling
My module will check for wrong input, especially when using logarithmic scales. 
Zero and negative values cannot be used with a logarithmic scale, so the program should give an error when they are entered.

ROAD MAP
Week 1 (Oct 1 - Oct 4): Finish this research file, learn about linear and logarithmic scales and tick generation, 
and agree on the scale, ticks, Figure, and Axes interfaces.
Week 2 (Oct 5 - Oct 11): Create the basic linear scale, reverse mapping, and start working on the Figure and Axes.
Week 3 (Oct 12 - Oct 18): Complete the Figure and Axes, margins, plot areas, tick generation, and the required tests.
Week 4 (Oct 19 - Oct 25): Add logarithmic scaling, handle wrong values, and support more than one Axes for the joint plot.
Week 5 (Oct 26 - Oct 31): Complete the documentation, test my work with the other modules, fix errors, and prepare for the code freeze.

How AI was useful
I used ChatGPT to help me understand axis scaling, linear and logarithmic scales, tick generation, Figure and Axes, 
and how my part connects with the other members' work.
I will declare the use of AI according to the project rules.
