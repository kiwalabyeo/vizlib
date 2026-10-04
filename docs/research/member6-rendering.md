# NAMBALIRWA FLAVIA CATE 
# 25/U/0848
# BELE
# GROUP 7 : Member 6 research

# Plot Rendering research 

## 1. What my module does

My module is the Canvas. It is an image kept in memory as a grid of RGBA pixels (Red, Green, Blue, Alpha, where alpha is transparency). Other modules tell it to draw lines, rectangles and circles, and it colours the right pixels. It also holds the shared types Color, Point and Rect.

## 2. How it works

Every part of our plots can be built from three shapes, so the Canvas only needs these:

- Lines: axes, ticks, gridlines, ECDF steps, rug ticks, and smooth curves made of many short lines.
- Rectangles (outline and filled): plot background, legend box, joint plot panels, histogram bars.
- Circles (outline and filled): scatter plot points.

Text is drawn by Member 8 on top of my Canvas. Keeping the shapes to a few simple ones means less code to test, and a new plot type later can reuse them.

Methods from the plan: width, height, set_pixel, get_pixel, draw_line, draw_rect, fill_rect, draw_circle, fill_circle.

Drawing a line: a pixel image can only colour whole pixels, so a diagonal line has to be approximated. Bresenham's algorithm does this with only whole-number arithmetic. It steps along the line and picks the pixel closest to the true line each time. Wu's algorithm is a smoother version that colours two neighbouring pixels with partial transparency to reduce jagged edges. I will do Bresenham first and Wu only if there is time.

Thick lines: draw several parallel lines, or a small filled shape at each point.

Circles: a midpoint circle algorithm can be used, which also uses only whole numbers. For a filled circle, I would draw horizontal lines between the left and right edges.

Clipping: if a shape goes outside the canvas, only the pixels inside are drawn, so nothing crashes or writes outside the image.

Bad input: a canvas with a width or height of zero or less, or a negative radius or thickness, should throw std::invalid_argument, as the group agreed.

## 3. Libraries or tools

None for my module, only the C++17 standard library. Member 7 uses stb_image_write to save the PNG, and Member 8 uses stb_truetype for text. Both of them use my Canvas but do not add any library to it.

## 4. What I need from others

Almost nothing, so I can start on day one. I only need the group to agree the shared types (Color, Point, Rect) and the Canvas method names in the interfaces PR due Wednesday 7 October, and how errors are reported.

## 5. What others need from me

- Member 3 (ECDF and rug) and Member 4 (scatter, histogram, joint plot) call my drawing methods to draw their shapes.
- Member 5 (Axes) uses the Rect type for the plot area and the Canvas to draw the axes and ticks.
- Member 7 reads the pixels of my Canvas to save the PNG.
- Member 8 draws text onto my Canvas, using set_pixel or the Color type.
- Everyone needs Color, Point and Rect.

## 6. Roadmap

- Week 1 (to 4 Oct): research, and draft the Color, Point, Rect and Canvas headers.
- Week 2 (5 to 11 Oct): interfaces PR merged by 7 Oct. Canvas with set_pixel and get_pixel, Bresenham lines and rectangles, with tests. Goal by Sunday: draw a line and save it as a PNG with Member 7.
- Week 3 (12 to 18 Oct): circles, filled shapes, line thickness and clipping, with tests. Help Members 3, 5 and 8 use the Canvas.
- Week 4 (19 to 25 Oct): Wu smooth lines if time allows, invalid input and edge case tests, and docs/canvas.md.
- Week 5 (26 to 31 Oct): example program, final docs, bug fixes, code freeze on 31 Oct.

## 7. Sources
- Bresenham's line algorithm (Wikipedia): https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
- Xiaolin Wu's line algorithm (Wikipedia): https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
- Midpoint circle algorithm (Wikipedia): https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
- std::invalid_argument (cppreference): https://en.cppreference.com/w/cpp/error/invalid_argument
- Group 7 Data Visualization Library Project Plan (for the module split and interfaces)

## 8. AI use

- Tools: Claude (Anthropic) and Gemini
- Purpose: explaining how the Canvas and the line, circle and clipping methods work, and helping to draft and tidy this research note
- Reason: to understand the topic before designing my module, and to check my note against the group's plan