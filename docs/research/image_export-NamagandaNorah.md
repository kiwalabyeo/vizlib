## NAMAGANDA NORAH MARGRET 25/U/0846
# Design and Implementation of a Data Visualisation Library in C++
**Project Scope:** Data loading, data preprocessing, statistical summaries, ECDF plots, rug plots, axis scaling, labels and annotations, plot rendering, and image export

---

This week, the group divided the project into different roles and assigned each member their responsibilities.
## Project roles
# Kizito James (Member 1)        - Data loading and preprocessing
# Natukunda Melissa (Member 2)   - Statistical summaries
# Crystal Zawedde (Member 3)     - ECDF and rug plots
# Kiwalabye Oscar (Member 4)     - Joint plots
# Mugabe Destiny  (Member 5)     - Axis scaling and layout
# Namalirwa F. Cate (Member 6)   - Rendering
# Namaganda Norah (Member 7)     - Image export, build and integration
# Kabahamba Joy (Member 8)       - Labels and Annotations

---

**Role:**Image Export, Build and Integration 

Our project is **Design and Implementation of a Data Visualisation Library in C++**. The library will support data loading, data preprocessing, statistical summaries, ECDF plots, rug plots, axis scaling, labels and annotations, plot rendering, and image export.

My role is **Image Export, Build and Integration**. It connects the different parts of the library and makes sure that the final visualisation can be saved as an image and that the whole project can build and pass its tests.

My main responsibilities are:

* Implement `save_png()` using **stb_image_write**.
* Optionally provide `save_ppm()` as a simple zero-dependency backup.
* Set up the main `CMakeLists.txt` and the CMake files for tests and examples.
* Keep the required third-party libraries, such as `stb_image_write` and `doctest`, in `third_party`.
* Set up GitHub Actions CI so that the project is built and tested automatically on pull requests.
* Create the full `pipeline.cpp` example that connects the modules from CSV input to a PNG output.
* Test the project on a fresh machine/environment before submission.

The main files I will work on include:

```text
include/vizlib/image_export.hpp
src/image_export.cpp
CMakeLists.txt
tests/CMakeLists.txt
examples/CMakeLists.txt
.github/workflows/ci.yml
tests/test_image_export.cpp
examples/pipeline.cpp
third_party/
```

## 2. How it works

### Image export

The rendering module will produce a canvas containing the final plot. My image export module will take this canvas and write its pixel data to an image file.

For PNG export, I will use **stb_image_write**, which is a small single-header library. The basic process will be:

1. Receive the canvas/pixel data from the rendering module.
2. Get the image width and height.
3. Pass the pixel data to `stb_image_write`.
4. Save the result as a `.png` file.

If implemented, `save_ppm()` will provide a simpler backup format that does not require an image library.

### Build and integration

I will use **CMake** to describe which source files belong to the library, tests, and examples. CMake then creates the build files used by the compiler. The project will use C++17 so that the modules follow the same C++ standard.

The normal build and test process will be:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The first command configures the project, the second builds the library and tests, and the third runs the tests.

### GitHub Actions CI

GitHub Actions will run the build and tests on a clean machine whenever a pull request is made to `main`. This helps us identify problems that may only work on one person's computer. The CI process will configure, build and test the project using the same basic steps as the local build.

## 3. Algorithms and methods I will use

The main methods in my module are relatively straightforward:

* **PNG writing:** pass the canvas pixel buffer, width, height and number of colour channels to `stb_image_write`.
* **PPM writing:** write the PPM header followed by the RGB pixel values.
* **Build integration:** add each source and test file to the appropriate CMake file.
* **Testing:** create a canvas, export it, then check that the image file was created and has the expected dimensions/size.
* **CI:** automatically configure, compile and run the tests after a pull request.

The image export itself does not calculate the plot. It mainly converts the already-rendered canvas into a file that can be viewed outside the library.

## 4. Libraries and tools

I will use:

* **C++17** – programming language standard for the project.
* **CMake** – project configuration and build system.
* **stb_image_write** – PNG image writing.
* **doctest** – unit testing.
* **CTest** – running the registered tests.
* **GitHub Actions** – continuous integration.
* **Git/GitHub** – version control and collaboration.

The project guide specifies that `doctest` is kept as a single header in `third_party`, so users do not need to install it separately.

## 5. What I need from other members

My module depends on the other modules being clear about their interfaces.

I need:

* The **plot rendering member** to provide the final canvas/pixel buffer, together with its width and height and the agreed pixel format.
* The **data loading member** to make sure the CSV data can reach the full pipeline example.
* The **preprocessing and statistical summary members** to provide the processed data needed by the plotting modules.
* The **ECDF, rug plot, axis scaling, labels and annotations members** to provide their functionality through the agreed interfaces.
* All members to tell me when they add or change a `.cpp` file or test file so that the appropriate CMake file can be updated.

The build guide also states that every new `.cpp` file needs to be added to the library's CMake configuration, while every new test file needs to be added to the tests CMake file.

## 6. What other members need from me

Other members mainly need:

* A working `save_png()` function.
* A clear interface in `image_export.hpp`.
* A stable way of exporting the canvas produced by the rendering module.
* A working build system so their modules can be compiled together.
* Tests and CI that help identify integration problems.
* The `pipeline.cpp` example showing how the complete library is used from CSV input to PNG output.

This should allow the other members to test their modules as part of the complete library instead of testing everything separately.

## 7. Roadmap for my role

### Week 1

* We divided the project roles among group members.

### Week 2 

* Study the existing project structure and the build/CI guide.
* Agree on the canvas/pixel format with the rendering member.
* Set up the initial image export files.
* Implement `save_png()` using `stb_image_write`.
* Implement `save_ppm()` if needed.
* Create `test_image_export.cpp`.
* Test saving a simple canvas and checking that the output file exists and has the expected size.

### Week 3 

* Complete the root `CMakeLists.txt`.
* Update the CMake files for tests and examples.
* Add the required third-party libraries.
* Create the initial `pipeline.cpp`.
* Integrate the different modules into the pipeline.

### Week 4 

* Complete GitHub Actions CI.
* Test the build and tests on a clean/fresh environment.
* Fix integration and build problems.
* Run the complete CSV-to-PNG pipeline.
* Check that no required files or dependencies are missing.

### Week 5

* Perform the final fresh-machine test.
* Confirm that the project builds and all tests pass.
* Check the generated PNG output.
* Check the repository structure and documentation.
* Make the final changes needed before submission.

## 8. Sources and links

* **stb_image_write:** GitHub repository for the image-writing library.
* **CMake documentation:** used to understand project configuration and building.
* **doctest:** used for writing the unit tests.
* **GitHub Actions documentation:** used for setting up CI.
* **Project Build, Tests and CI Guide:** used as the main guide for our project's CMake, testing and CI structure.

## 9. AI use

I used **ChatGPT** to help me understand the integration requirements and organise my work.

I used AI mainly for:

* Understanding how CMake connects the library, tests and examples.
* Understanding how GitHub Actions CI should build and test the project.
* Understanding how `stb_image_write` can be used for PNG export.
* Helping organise the module roadmap and documentation.

AI was used as a learning and planning tool, but the code will be tested and checked by me and integrated with the group's actual project. The group guide also requires AI assistance to be declared in the Week 1 report.
