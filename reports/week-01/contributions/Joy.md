# MEMBER 8 - KABAHAMBA JOY A MARIA 25/U/0830
# Design and Implementation of a Data Visualisation Library in C++

**Project Scope:** Data loading, data preprocessing, statistical summaries, ECDF plots, rug plots, axis scaling, labels and annotations, plot rendering, and image export

---

**Role:** Labels and Annotations

Our project is **Design and Implementation of a Data Visualisation Library in C++**. The library will support data loading, data preprocessing, statistical summaries, ECDF plots, rug plots, axis scaling, labels and annotations, plot rendering, and image export.

My role is **Labels and Annotations**. I am responsible for the text functionality used in the library and for adding readable information to plots, such as titles, axis labels, tick labels, legends, and free-text annotations.

My main responsibilities are:

* Implement text drawing using **stb_truetype**.
* Load an open-licence font such as **DejaVu Sans** from the `third_party` folder.
* Provide functionality for drawing text onto the Canvas at a specified position, size, and colour.
* Provide plot titles, x-axis and y-axis labels, and tick labels.
* Provide legends and free-text annotations placed on plots.
* Write tests for text drawing and annotations.
* Write the documentation for the labels and annotations module.
* Provide at least one example showing the use of the module.

The main files I will work on include:

```text
include/vizlib/text.hpp
include/vizlib/annotations.hpp
include/vizlib/legend.hpp

src/text.cpp
src/annotations.cpp
src/legend.cpp

tests/test_text.cpp
tests/test_annotations.cpp

docs/text.md
docs/research/member8-labels.md
```

## 2. How it works

### Text drawing

The text module will use **stb_truetype** to load the project's font and render characters onto the Canvas. The text drawing interface will receive a string, position, size, and colour and use these values to determine how the text is drawn.

The basic process will be:

1. Load the font from the agreed font file in `third_party`.
2. Receive the text string that needs to be displayed.
3. Determine the position and size of the text.
4. Render the characters onto the Canvas.
5. Use the selected colour when drawing the text.

The text functionality will provide the basic drawing operation used by the annotation and legend modules.

### Annotations

The annotations module will use the text drawing functionality to add information to plots. This includes plot titles, x-axis labels, y-axis labels, tick labels, and free-text notes.

The position of these labels must work with the **Axes and layout system from Member 5**, so that the text appears in the correct place relative to the plot.

### Legends

The legend module will provide a way of explaining the graphical elements used in a plot. A legend can contain a label together with the corresponding line, marker, or other graphical representation.

The legend will use the Canvas and text functionality to display the legend in an appropriate position on the plot.

## 3. Algorithms and methods I will use

The main methods in my module are:

* **Font rendering:** use `stb_truetype` to convert font characters into pixels that can be drawn onto the Canvas.
* **Text positioning:** use a position supplied by the caller and the layout information from the Axes system to place text correctly.
* **Text measurement:** determine the width and height of text so that labels can be positioned and tested correctly.
* **Annotations:** place titles, axis labels, tick labels, and free-text notes using the text drawing functionality.
* **Legends:** combine text labels with graphical symbols or line representations to explain plotted data.
* **Testing:** check text dimensions and confirm that drawing text changes the expected Canvas pixels.

The text module does not calculate the statistical data or create the actual plots. It provides the text and labelling functionality that makes the generated plots understandable.

## 4. Libraries and tools

I will use:

* **C++17** – programming language standard for the project.
* **stb_truetype** – used for loading the font and rendering text.
* **DejaVu Sans** or another agreed open-licence font – used as the project's font.
* **Canvas** – used as the drawing surface for the rendered text.
* **doctest** – used for unit testing.
* **Git/GitHub** – used for version control and collaboration.

The project guide specifies that the required libraries and font files are kept inside the repository, so the project does not depend on external installations.

The project does not use **OpenGL** or **JSON**.

## 5. What I need from other members

My module depends mainly on the **Canvas and Axes** interfaces being agreed.

I need:

* **Member 6:** the Canvas interface and drawing operations so that text can be rendered onto the image.
* **Member 5:** the Axes and layout information so that titles, axis labels, tick labels, annotations, and legends can be positioned correctly.
* **Member 7:** the build system to include my source and test files when the project is integrated.
* The group: agreement on the public interfaces, especially the `draw_text` function and how positions, colours, sizes, and errors are represented.

The project guide specifies a `draw_text` function that takes a string, position, size, and colour as part of the agreed interfaces.

## 6. What other members need from me

Other members mainly need:

* A working text-drawing interface.
* A clear `text.hpp` interface that can be used by the annotation and legend modules.
* Functionality for plot titles and axis labels.
* Functionality for tick labels and free-text annotations.
* A legend interface for explaining graphical elements.
* Tests showing that text is rendered correctly.
* Documentation explaining how the public functions are used.

The plotting members will depend on these functions when their plots need titles, labels, tick labels, annotations, or legends.

## 7. Roadmap for my role

### Week 1

* Research the labels and annotations requirements.
* Study how `stb_truetype` can be used for text rendering.
* Decide what functionality is needed for text, annotations, and legends.
* Create and submit the research file.

### Week 2

* Agree on the interfaces for `text.hpp`, `annotations.hpp`, and `legend.hpp`.
* Agree with Member 6 on how text will interact with the Canvas.
* Agree with Member 5 on positioning and layout requirements.
* Begin the basic text drawing implementation.

### Week 3

* Implement the first working text drawing functionality.
* Add text measurement such as width and height.
* Write `test_text.cpp`.
* Test that drawing text changes the expected Canvas pixels.

### Week 4

* Implement plot titles, axis labels, tick labels, and free-text annotations.
* Implement the legend functionality.
* Add `test_annotations.cpp` and the required tests.
* Test the positioning of labels and annotations with the Axes system.

### Week 5

* Complete the text, annotations, and legend functionality.
* Complete module documentation and the example.
* Fix integration issues with the Canvas and Axes modules.
* Make sure the module works correctly as part of the complete library before the code freeze.

## 8. Sources and links

* **stb_truetype:** GitHub repository for the stb single-header libraries.
* **DejaVu Fonts:** information about the open-licence font used by the project.
* **Project Plan:** used to understand the responsibilities, interfaces, dependencies, tests, and timeline for Member 8.
* **Project Research and Review Process:** used as the guide for the required research file structure and submission process.

## 9. AI use

I used **ChatGPT** to help me understand the requirements of the Member 8 module and organise my research.

I used AI mainly for:

* Understanding the role of text drawing, annotations, and legends in the project.
* Understanding how my module depends on the Canvas and Axes modules.
* Organising the research file according to the required project structure.
* Clarifying the purpose of the files assigned to Member 8.

AI was used as a learning and planning tool. I will make sure that I understand and can explain the work I contribute to the project, and that AI use is declared according to the group's project rules.
