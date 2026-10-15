# vizlib: Week 2 Group Report

**Group:** 7  |  

## Tasks completed

- **Kizito Mark James:** `dataframe`, `csv_reader` and `preprocessing` (with `drop_missing()`) headers and sources; sample data `tips.csv`.
- **Melissa Natukunda:** header and source skeleton for the `summaryStatistics` class (mean, median, mode, range, standard deviation, quantile, interquartile range, count).
- **Flavia Cate:** `color.hpp`, `geometry.hpp` and `canvas.hpp` (`Color`, `Point`, `Rect`, `Canvas`).
- **Joy:** reviewed the roadmap, the Canvas work and the axis-scaling README.
- **Namaganda Norah:** `save_png()` and `save_ppm()` with tests, in PR `#48`.
- **Mugabe Destiny:** work started, nothing finished yet.

## Tasks in progress

- **James:** unit tests for the DataFrame and CSV reader.
- **Melissa:** remaining statistics functions and integration into the library.
- **Cate:** `canvas.cpp`, its tests, and `draw_line` (DDA first).
- **Destiny:** linear scaling, reverse mapping, and the `Figure` and `Axes` classes; `ticks` next.
- **Joy:** planning the interfaces for `text.hpp`, `annotations.hpp` and `legend.hpp`.
- **Norah:** the Canvas export adapter, waiting for `canvas.cpp` to reach `main`.

## Challenges faced

- **James:** catching up on the Week 1 planning while coding, and learning Git branching and pull requests.
- **Melissa:** could not install C++23, and online compilers are unreliable because of unstable campus internet.
- **Cate:** choosing `int` or `double` for `Point` and `Rect`, agreeing which exception `get_pixel` should throw, and understanding the Bresenham line algorithm.
- **Destiny:** code not run yet; her work must fit the other members' Canvas and `Rect`.
- **Joy:** understanding the interfaces needed to connect with the other modules.
- **Norah:** setting up CMake and a C++ compiler, and `canvas.cpp` not being on `main` yet.

## AI tools used

- **Claude:** Cate (explanations, header review), Melissa (header skeleton), Norah (guidance and drafting).
- **Gemini:** James (header design, research structure), Melissa (mode function help).
- **ChatGPT:** Destiny and Joy (understanding requirements and structuring modules).

## References

- C++ file streams: https://en.cppreference.com/w/cpp/io/basic_ifstream
- `std::stod`: https://en.cppreference.com/w/cpp/string/basic_string/stof
- `std::invalid_argument`: https://en.cppreference.com/w/cpp/error/invalid_argument
- Finding duplicates in a vector (used for `mode`): https://www.geeksforgeeks.org/cpp/how-to-find-duplicates-in-a-vector-in-cpp/
- stb single-file libraries (`stb_image_write`, `stb_truetype`): https://github.com/nothings/stb
- Group 7 Data Visualization Library project plan
