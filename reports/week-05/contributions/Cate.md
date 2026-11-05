Week 05 Contribution Report --- Canvas / Plot Rendering
Contributor: Nambalirwa Flavia Cate
Registration Number: 25/U/0848
Group: 7
Role: Member 6 --- Canvas / Plot Rendering
Project: VIZLIB
Reporting period: Week 05
1. Introduction
During Week 05, my main responsibility was to bring the Canvas rendering
module to a reliable and well-documented state. The Canvas module
provides the drawing surface on which plots and other graphical elements
can be rendered. In simple terms, it is the part of VIZLIB that turns
drawing instructions into coloured pixels.

My focus this week was not simply to have the drawing functions in the
source code. I also needed to check that they behaved as expected,
including when they received unusual inputs or were asked to draw close
to, or beyond, the edges of the canvas. I therefore concentrated on
completing the tests, carrying out hand-picked pixel verification,
checking boundary behaviour and documenting the module.

By the end of the week, I had completed the Canvas testing and
pixel-verification work. I also prepared a general report explaining the
Canvas module and saved it in the project's docs/modules directory.
That report is intended to help another developer or group member
understand the module without having to begin by reading every line of
the implementation.
2. Week 05 responsibilities
My responsibilities as Member 6 were focused on the Canvas and
plot-rendering part of VIZLIB.
  Area                                Work completed
  Canvas implementation               Checked lines, rectangles, filled
rectangles, circles and filled
circles.

  Line thickness                      Tested the effect of thickness on
the pixels covered by a line.

  Clipping                            Checked drawing at and beyond the
canvas boundaries.

  Automated tests                     Completed and ran
tests/test_canvas.cpp.

  Pixel verification                  Checked selected pixel positions to
confirm that drawing operations
produced the expected results.

  Documentation                       Prepared a general Canvas module
report and saved it in
docs/modules.
Integration                         Reviewed the Canvas interface and
its role as the drawing surface
used by plotting components.
These tasks are connected. For example, it is not enough for a
line-drawing function to work in an ordinary situation if it fails when
the line reaches the edge of the image. Likewise, a test that only
checks whether a function runs does not necessarily prove that it
colours the correct pixels. I therefore treated testing and pixel
verification as important parts of completing the module.
3. Checking the drawing operations
3.1 Lines
Line drawing is one of the basic operations because many graphical
elements depend on straight edges. I checked the line-rendering
behaviour using different endpoint positions and directions, including
lines that approach the canvas boundaries.

I also checked line thickness. A line with a thickness greater than one
should cover more than a single row or column of pixels. I looked at the
pixels around the line path rather than assuming that the thickness
argument was being applied correctly.

The implementation uses Bresenham's line algorithm. It selects a
sequence of discrete pixel positions that approximates the mathematical
line between two endpoints. This is useful in raster graphics because a
digital canvas is a grid of pixels, not a continuous drawing surface.
3.2 Rectangles and filled rectangles
I checked both rectangle outlines and filled rectangles. They serve
different purposes:
- draw_rect draws the boundary of a rectangle.
- fill_rect colours the area inside the rectangle.
For the outline operation, I checked that the edges appeared in the
expected positions. For the filled operation, I checked pixels inside
and outside the rectangle. This distinction matters because an outline
should not automatically colour the whole interior.

I also considered rectangles that meet or extend beyond the canvas
boundaries. The visible part of a shape should be handled safely,
without attempting to access invalid pixel positions.
3.3 Circles and filled circles
I checked the two circle operations:
- draw_circle draws the circumference.
- fill_circle colours the interior.
The outline implementation uses the midpoint circle approach and takes
advantage of circle symmetry to calculate corresponding points around
the circumference. The filled-circle operation works across visible
horizontal rows and determines the horizontal span belonging to the
circle on each row.

During verification, I checked representative pixels on the outline,
inside the filled region and outside the expected shape. This helped
distinguish the behaviour of the outline and fill operations.
4. Line thickness and clipping
4.1 Line thickness
Thickness changes the number of pixels used to represent a line. I
checked that a thickness greater than one actually affected the rendered
pixels. This was important because a line can look thicker at first
glance while still having errors in the placement of its extra pixels.
4.2 Boundary handling
A Canvas has a fixed width and height. Valid pixel coordinates run from
zero to one less than the corresponding dimension. Drawing functions may
nevertheless receive coordinates outside that range.

I checked how drawing operations behave at the boundaries and when
shapes extend beyond them. The intended behaviour is safe handling of
invalid pixel positions. For example, set_pixel ignores coordinates
outside the canvas, while filled-shape operations restrict their work to
the visible canvas area where applicable.

This is important to the reliability of the library. Plotting components
may calculate positions close to an image edge, and the drawing layer
should handle those positions without writing outside the pixel array.
5. Automated testing
I completed and ran the Canvas test suite in tests/test_canvas.cpp
during Week 05. The tests were used to check actual behaviour, not
merely whether the project compiled.

The testing covered the ordinary use of the drawing functions as well as
edge cases and invalid inputs. Areas checked included:
- Canvas dimensions and basic pixel access.
- Setting and retrieving pixel colours.
- Line drawing and line thickness.
- Rectangle outlines and filled rectangles.
- Circle outlines and filled circles.
- Drawing close to or beyond canvas boundaries.
- Invalid dimensions and invalid drawing parameters.
- Cases where an operation should safely do nothing, reject an input
  or avoid writing outside the canvas.
I also built the project and ran the registered tests. The build
completed, and the CTest run reported that all registered tests passed.

The command used was:
ctest --test-dir build --output-on-failure
One detail worth explaining is that CTest may report one registered test
because the project runs the individual checks through a test
executable. That number should not be confused with the number of
individual test cases inside that executable.
6. Hand-picked pixel verification
Alongside automated testing, I completed hand-picked pixel verification.
This was important because drawing is ultimately about the pixels stored
in the Canvas.

I selected particular coordinates and checked their colours after
drawing operations. Depending on the shape, I checked pixels that should
have been coloured, pixels that should have remained unchanged and
pixels close to the shape's boundary.

For example, when checking a filled rectangle, examining a pixel inside
the rectangle and another outside it helps establish whether the fill is
occurring in the correct region. For a circle, checking a point inside
the filled area, a point on or near the outline and a point outside the
circle helps distinguish the expected behaviour of the two circle
operations.

The purpose was to verify the result itself, rather than merely
confirming that a drawing function executed without an error. This also
reinforced an important idea in raster graphics: a mathematical line or
circle is continuous, but a Canvas stores a finite grid of discrete
pixels.

The pixel-verification work was completed during Week 05.
7. Documentation completed
An important part of my Week 05 work was documentation. I prepared a
general Canvas module report and saved it in:
docs/modules/
The report is intended to explain the Canvas module as a whole, rather
than simply list its function names. It starts with the module's purpose
and overall structure before explaining the shared types, public
interface, drawing operations, algorithms, pixel storage, validation,
boundary handling, testing and design considerations.

I wanted the documentation to be useful to someone approaching the
module for the first time. For that reason, the explanation moves from
the general picture to the individual components and then into the
technical details. This gives the reader some context before introducing
implementation-level information.

The general module report is a separate document from this weekly
contribution report. This file records what I worked on during Week 05,
while the report in docs/modules serves as a broader guide to the
Canvas module.
8. Canvas in the wider VIZLIB project
Canvas is a lower-level part of the library. It provides a common
drawing surface and basic rendering operations that other plotting
components can use to draw the visual parts of their plots.

This means that the Canvas interface needs to be consistent and
understandable. A plotting component should be able to use the drawing
functions without needing to manage the underlying pixel array directly.

During Week 05, I reviewed the Canvas interface in this wider context
and considered how the module fits into the rendering process. My
contribution remained focused on Canvas rendering. Other group members
had separate responsibilities, including other plotting components and
image export. Drawing shapes on the Canvas and saving the resulting
image are related stages, but they are not the same task.
9. Challenges and careful considerations
One thing I had to keep in mind is that drawing functions can appear to
work in ordinary cases while still having problems in less obvious
situations. A line drawn entirely inside the canvas is relatively
straightforward. A line that reaches an edge, extends beyond an edge or
has a larger thickness requires more careful handling.

Filled shapes also need to stay within the valid pixel range even when
their mathematical dimensions extend beyond the image. Invalid inputs
require predictable behaviour too. Depending on the operation and the
input, that may mean rejecting the input, doing nothing for an empty
shape or safely ignoring pixels outside the canvas.

The pixel checks made these issues easier to reason about. Instead of
relying only on how a shape looked, I could inspect the colour stored at
selected coordinates. This made the verification more precise.
10. Week 05 outcomes
By the end of Week 05, I had completed the Canvas work assigned to me in
the areas of implementation checking, testing, pixel verification and
documentation.

The main outcomes were:
1. The Canvas drawing operations were checked, including lines,
   rectangles, filled rectangles, circles and filled circles.
2. Line thickness was checked against the pixels covered by the
   rendered line.
3. Boundary and clipping behaviour was checked for drawing at and
   beyond the canvas edges.
4. The tests in tests/test_canvas.cpp were completed and run.
5. Hand-picked pixel verification was completed to check the actual
   output of drawing operations.
6. The project was built and the registered CTest run completed
   successfully.
7. A general explanatory Canvas module report was prepared and saved in
   docs/modules.
8. The Canvas interface and its role as a shared drawing surface were
   reviewed in relation to the other plotting components.
My contribution was therefore not limited to having the code in place. I
also checked the behaviour of the module and documented it so that other
group members can understand and maintain the work.
11. Personal reflection
This week helped me appreciate that rendering is not simply a matter of
writing a function that draws a shape. The function also has to behave
properly when the input is unusual, when a shape reaches the edge of the
image and when the requested thickness or dimensions make the drawing
more complicated.

I found the pixel-verification part especially useful because it made me
look beyond the general appearance of a drawing. A shape can look
reasonable while still having individual pixels in the wrong positions.
Checking selected coordinates gave me a more direct way to establish
whether the operation was doing what it was supposed to do.

I also realised the value of documentation. When working on a module, it
is easy to become familiar with the code and forget that another person
may not know why a particular function exists or how it should be used.
Preparing the general Canvas report encouraged me to explain the module
from the reader's point of view, starting with the overall purpose and
then moving into the details.

Overall, Week 05 was about checking, confirming and explaining the
Canvas work. I completed the testing and pixel-verification tasks and
prepared the general module documentation. I wanted the work to be
useful not only to me as the person implementing it, but also to the
next person who needs to understand or use the Canvas module.
12. Files and evidence
  File or directory                   Purpose
  src/canvas.cpp                    Canvas rendering implementation.

  include/vizlib/canvas.hpp         Public Canvas interface.

  include/vizlib/color.hpp          RGBA colour type used by Canvas.

  include/vizlib/geometry.hpp       Shared point and rectangle geometry
types.

  tests/test_canvas.cpp             Automated tests for Canvas
behaviour.

  docs/modules/                     Location of the general Canvas
module report.
build/                            CMake build directory used for
compiling and running tests.
Build command:
cmake --build build
Test command:
ctest --test-dir build --output-on-failure
This report records the Week 05 contribution. The general Canvas module
report is a separate document saved in docs/modules.