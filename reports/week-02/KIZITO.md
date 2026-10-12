KIZITO MARK JAMES

25/U/30991/PS

BCCE

## Introduction
Week 2 focused on building the core foundational interfaces and implementation files for the vizlib library, specifically establishing data ingestion and preprocessing capabilities.

## What Was Done This Week

Developed and finalized dataframe.hpp and dataframe.cpp to support columnar data storage and name-based column retrieval.

Implemented csv_reader.hpp and csv_reader.cpp to read CSV files from disk, parse lines, handle missing values, and throw appropriate standard exceptions on bad input.

Created preprocessing.hpp and preprocessing.cpp incorporating the drop_missing() function to filter out incomplete data rows.

Added sample CSV files like tips.csv to the data/input/ directory so downstream team members can test statistical summaries and plotting modules.

Participated in team coordination meetings and aligned module interfaces with group specifications.

## Challenges Faced

Catching up on missed Week 1 architectural planning while simultaneously implementing Week 2 data loading and preprocessing code.

Learning and navigating Git branching workflows, commit conventions, and pull request submissions for team review.

## Next Steps

Write comprehensive unit tests for the DataFrame and CSV reader modules using doctest.h.

Support team members in integrating statistical and plotting modules with the DataFrame output.

Refine documentation and prepare code for final peer review and merging into the main branch.

## 1. What my module does
My module is responsible for reading tabular data from CSV files and constructing the core DataFrame and Column data structures for the vizlib library.
It parses text lines, handles missing or malformed values gracefully, converts numerical fields into strongly-typed representations (std::optional<double>), and provides cleaning utilities.

## Main Responsibilities

- Research documentation: Author and commit the individual module research markdown document detailing planned C++ tools, architecture, and pipeline dependencies.
- Interface coordination: Collaborate with team members to finalize core header signatures for the DataFrame and CSV reader before writing logic.
- DataFrame architecture: Implement columnar data structures in dataframe.hpp and dataframe.cpp to manage memory storage and name-based column lookups.
- CSV ingestion engine: Develop csv_reader.hpp and csv_reader.cpp to open file streams, extract headers, and convert tokens while handling bad input and edge cases.
- Preprocessing utilities: Implement data cleaning functions such as drop_missing() to filter incomplete records.
- Sample data integration: Populate the data/input/ directory with sample files like tips.csv to enable team-wide testing.

## 2. How it works

- CSV Ingestion: Opens a file stream using std::ifstream, reads line by line with std::getline(), and splits rows by comma delimiters while trimming whitespace.
- Header Parsing: Uses the first row to determine column names and dictionary indices.
- Parsing & Type Conversion: Converts raw string fields into double values using std::stod. Empty fields, "NA", or unparseable values are wrapped in std::optional<double> as missing values (std::nullopt).
- Data Preprocessing: Provides a drop_missing() function that scans all rows and returns a clean DataFrame containing only complete records without missing fields.

## 3. Libraries or tools

- Standard C++17 Library
Using <fstream>, <sstream>, <vector>, <string>, <unordered_map>, <optional>, and <stdexcept>.
- No external dependencies
Built using pure standard library components without OpenGL, JSON libraries, or external parser dependencies.

## 4. What I need from others

- None. Data loading is the entry point of the entire data visualization pipeline, so this module does not depend on any other team member's code.

## 5. What others need from me

- Member 2
  (Statistical Summaries): Depends on DataFrame and Column to compute mean, median, quantiles, and histogram bins.
- Members 3 & 4 (Plot Logic)
  Depend on DataFrame columns to feed numerical series into ECDF, rug, scatter, and joint plots.
- Member 7
  (Integration Example): Depends on vizlib::read_csv() to execute the end-to-end full_pipeline.cpp script.

## 6. Roadmap

- Week 1: Initial research and architecture planning / research.
- Week 2: Implement dataframe.hpp, csv_reader.hpp, preprocessing.hpp, and underlying C++ implementations; add sample tips.csv dataset.
- Week 3: Unit testing edge cases (malformed files, missing columns, empty rows) using doctest.
- Week 4 & 5: Pipeline integration support, documentation polish, and error handling.

## 7. Sources

- C++ File Streams & Input Parsing (cppreference): https://en.cppreference.com/w/cpp/io/basic_ifstream
- std::stod & Exception Handling (cppreference): https://en.cppreference.com/w/cpp/string/basic_string/stof
- std::invalid_argument & std::runtime_error (cppreference): https://en.cppreference.com/w/cpp/error/invalid_argument
- Group 7 Data Visualization Library Project Plan (for the module split and interfaces).

## 8. AI use

- Tool: Gemini / AI Assistant
- What for: Assisting with standard C++17 std::optional interface design and drafting research markdown structure.
- Why: To streamline C++ header design according to group project requirements and fast-track repository contribution.
