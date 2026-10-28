# Week 03 Report: Member 6, Plot Rendering

Name: Nambalirwa Flavia Cate
Reg No: 25/U/0848
Programme: BELE
Group 7, vizlib

# Week 3 Report: Canvas Drawing Operations

## 1. What I worked on this week

In Week 2, I mainly worked on the shared types and the basic Canvas
interface. For Week 3, I continued with the actual drawing operations.
The Canvas is an image held in the computer's memory. It is made up of
pixels, and each pixel stores a colour using red, green, blue and alpha
(RGBA) values.

The aim was to make the Canvas able to draw the basic shapes that the
other plotting modules need. I implemented the line, rectangle and
circle operations and added tests to check that they behave as expected.

## 2. Files involved

  -----------------------------------------------------------------------
  File                                Purpose
  ----------------------------------- -----------------------------------
  `include/vizlib/color.hpp`          Defines `Color`, which stores the
                                      red, green, blue and alpha values.

  `include/vizlib/geometry.hpp`       Defines `Point` for a position and
                                      `Rect` for a rectangular area.

  `include/vizlib/canvas.hpp`         Declares the `Canvas` class and its
                                      public functions.

  `src/canvas.cpp`                    Contains the implementation of the
                                      Canvas and drawing operations.

  `tests/test_canvas.cpp`             Tests pixel access, drawing shapes
                                      and some invalid inputs.

  `tests/CMakeLists.txt`              Includes the Canvas test file in
                                      the test program.
  -----------------------------------------------------------------------

## 3. What I implemented

### 3.1 Canvas and pixels

The `Canvas` stores its width, height and pixels. The pixels are kept in
a single `std::vector<Color>`. The constructor checks that the width and
height are positive, then fills the image with the background colour.

The public `width()` and `height()` functions let other code find out
the size of the Canvas. `set_pixel()` changes a pixel's colour. If a
position is outside the Canvas, it does nothing. This is useful when a
line or shape reaches beyond an image's edge. `get_pixel()` reads a
pixel, and it throws `std::out_of_range` if the requested position does
not exist.

### 3.2 Drawing lines

I implemented `draw_line()` using Bresenham's line algorithm. An image
is made of whole pixels, so a diagonal line cannot be drawn as a
perfectly continuous mathematical line. Bresenham's algorithm chooses
the pixels that best follow the line. It uses an integer error value to
decide when to move across or down the image.

The function also accepts a thickness value. A thickness greater than
one draws a wider line by colouring additional nearby pixels. A
thickness below one is rejected.

### 3.3 Drawing and filling rectangles

`draw_rect()` draws the four edges of a rectangle, leaving the inside
unfilled. `fill_rect()` colours the rectangle's interior. These
operations are useful for drawing things such as bars, plot panels and
boxes.

### 3.4 Drawing and filling circles

`draw_circle()` draws the outline of a circle using the midpoint circle
algorithm. `fill_circle()` colours the inside of a circle. These
operations are useful for scatter-plot markers and other round shapes. A
negative radius is rejected.

### 3.5 Keeping drawing within the Canvas

Drawing operations use `set_pixel()`, which ignores pixels outside the
image. This means that a shape can reach an edge without writing outside
the pixel array. The tests include a line that runs off the Canvas to
check that this does not cause a crash.

## 4. How I tested the work

I added `tests/test_canvas.cpp` with tests for the main Canvas functions. 
The tests check that:

-   A pixel can be set to a colour and read back correctly.
-   Setting a pixel outside the Canvas does not crash.
-   Creating a Canvas with a zero dimension throws an exception.
-   Horizontal and diagonal lines colour the expected pixels.
-   A line that runs beyond the Canvas edge does not crash.
-   A line thickness below one is rejected.
-   `fill_rect()` fills the rectangle, and `draw_rect()` draws an
    outline only.
-   `draw_circle()` draws an outline, and `fill_circle()` fills the
    circle.
-   A negative circle radius is rejected.

It was also something new I learnt that I realised makes life a lot easier: 
I configured and built the project in GitHub Codespaces using CMake. The build
 completed successfully, including compilation of
`tests/test_canvas.cpp`. I then ran:

``` bash
ctest --test-dir build --output-on-failure
```

CTest reported that the registered `vizlib_tests` program passed: **1/1
registered test programs passed**. This is the number of CTest programs,
not the number of individual doctest assertions.

## 5. The four principles of object-oriented programming

The four commonly taught principles are encapsulation, abstraction,
inheritance and polymorphism. They do not all have to appear in every
class. In my Canvas module, encapsulation and abstraction are directly
relevant. Inheritance and polymorphism are not currently used in the
Canvas class, and I explain why below.

### 5.1 Encapsulation --- used

Encapsulation means keeping the important data inside a class and
controlling how other code can access or change it.

In my `Canvas` class, `width_`, `height_` and `pixels_` are private.
Other modules cannot directly resize the pixel vector or change the
stored dimensions. Instead, they use public functions such as `width()`,
`height()`, `set_pixel()` and `get_pixel()`. This helps protect the
Canvas from accidental changes that could make its dimensions and pixel
storage disagree.

The `Color`, `Point` and `Rect` structures have public members because
they are simple data holders. For example, code needs to read and set a
point's `x` and `y` coordinates directly. They do not manage the
Canvas's internal pixel storage.

### 5.2 Abstraction --- used

Abstraction means providing simple operations while hiding the details
of how those operations are carried out.

A plotting module can call `draw_line()` without needing to know how
Bresenham's algorithm chooses each pixel. It can call `fill_circle()`
without needing to know how the circle's interior is calculated.
Similarly, another module can call `set_pixel()` without needing to
calculate the pixel's position inside the vector.

The public functions provide the operations that other modules need,
while the implementation details stay in `src/canvas.cpp`. This lets
other members use the Canvas without having to understand every drawing
algorithm.

### 5.3 Inheritance --- not used in the Canvas module

Inheritance is when one class is based on another class and receives or
extends its behaviour. For example, a class could inherit from a general
drawing-surface class.

Honestly, I did not apply it much in my specific part of the group work. 
My `Canvas` class does not inherit from another class, and no class in
my rendering module inherits from `Canvas`. That is reasonable for this
module because the project currently needs one concrete Canvas
implementation. Adding inheritance without a real need would make the
design more complicated.

### 5.4 Polymorphism --- not used directly in the Canvas module

Polymorphism allows the same interface or operation to behave
differently depending on the type of object involved. One common C++
example is calling an overridden virtual function through a base-class
reference or pointer.

My Canvas drawing functions are ordinary member functions. For example,
`draw_line()` uses the same line-drawing implementation whenever it is
called, so the Canvas module does not currently use run-time
polymorphism.

Other parts of the project may use inheritance and polymorphism for
different kinds of plots, but that is separate from the Canvas
implementation. I have not counted those as features implemented by my
module.

## 6. How my work supports the rest of the group

The Canvas is the shared drawing surface for the library. The plotting
modules can use its drawing functions instead of implementing their own
pixel operations. For example:

-   The scatter-plot module can use circles or filled pixels for data
    points.
-   The histogram module can use filled rectangles for bars.
-   The ECDF and rug modules can use lines.
-   The axes module can use lines and the shared `Point` and `Rect`
    types.
-   The text and PNG-export modules can work with the Canvas through the
    interfaces agreed by the group.

PNG export and the creation of a complete plot with axes and data are
integration tasks involving other modules. My main responsibility is to
provide drawing operations that those modules can use.

## 7. What is still pending or planned

The main drawing operations for this stage are implemented and the
current test program passes. The following items still need to be
checked or completed as part of the remaining project work:

-   Check that public functions in the headers have clear comments.
-   Complete or verify the rendering research note and the Canvas
    documentation page.
-   Verify that the Canvas works with the existing PNG-export function;
    I do not need to create a separate PNG writer if the project already
    provides one.
-   Coordinate with the group to check that the other plotting modules
    use the Canvas correctly.
-   Continue with additional boundary and invalid-input checks as
    planned for Week 4.
-   Confirm the feature branch, pull request and review status.

### 7.1 Improving the Existing Bresenham's Line Algorithm

For line drawing, our group originally chose Bresenham's line algorithm, 
as stated in the original work plan. I followed this decision when 
preparing my personal roadmap and implementing the current line-drawing functionality.

Along the way, I learnt about Xiaolin Wu's line algorithm, which can be 
used to produce smoother lines through antialiasing. Although this 
algorithm was earlier listed as an optional improvement in the original 
work plan, I do not plan to prioritise it for the current work. Instead, 
I will focus on improving the already implemented Bresenham's algorithm, 
particularly its line-thickness handling, clipping, and boundary conditions. 
This will allow me to improve the existing functionality while maintaining 
consistency with the group's original approach.
However, I will discuss my groupmates this week and we decide if our program shouuld include 
it or not, so that I do not make a redundant function which will not be implemented.

The YouTube video linked below helped me understand Bresenham's line-drawing algorithm and its implementation. Although the explanation uses Python, it helped me understand the underlying logic and concepts that I could apply when implementing the algorithm in C++.

https://www.youtube.com/watch?v=CceepU1vIKo&pp=ygUiYnJlc2VuaGFtJ3MgbGluZSBkcmF3aW5nIGFsZ29yaXRobQ%3D%3D

## 8. AI use

**Tools used and Purpose:** ChatGPT and Claude, mainly for understanding 
and better explananation as I was writing my code, and getting shorter and 
generally better ways of implementing certain parts of it. 
The AI was also helpful in organising this report from my project notes and 
the implementation and test results discussed.The formatting(numbering), advice to break 
down sentences into shorter lines rather than to long sentences was also learnt 
from AI, which was also done with shorter line length.

**Reason:** To present what I implemented, how I tested it and which
object-oriented principles are actually used, in straightforward
language.

I will check this report against my final code and the group's AI-use
requirements before submitting it.
