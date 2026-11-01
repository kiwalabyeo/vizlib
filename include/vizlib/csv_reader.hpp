// csv_reader.hpp - read a CSV file into a DataFrame (Member 1)
#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "vizlib/dataframe.hpp"

namespace vizlib {

// What to do with a row whose number of fields differs from the header.
enum class BadRowPolicy {
    Throw,  // throw std::invalid_argument naming the line (default)
    Skip    // silently drop the row
};

struct CsvOptions {
    char delimiter = ',';
    bool has_header = true;  // false: columns are named col0, col1, ...
    BadRowPolicy bad_rows = BadRowPolicy::Throw;
    // Unquoted cells equal to one of these (after trimming whitespace) are
    // missing. An empty cell is always missing.
    std::vector<std::string> na_values = {"NA", "N/A", "NaN", "nan", "null", "NULL"};
};

// Read a CSV file.
//
//  - Quoted fields may contain the delimiter, doubled quotes ("") and
//    line breaks. A UTF-8 byte order mark and CRLF line endings are handled.
//  - Blank lines are ignored.
//  - A column is numeric if every non-missing cell is a plain decimal number
//    (1, -2.5, 3e4); otherwise it is a text column. Quoted numbers such as
//    "16.99" count as numbers too.
//  - Empty cells and cells listed in options.na_values are missing.
//
// Throws std::runtime_error if the file cannot be opened.
// Throws std::invalid_argument if the content is bad: empty input, duplicate
// column names, an unterminated quote, or (with BadRowPolicy::Throw) a row
// with the wrong number of fields.
DataFrame read_csv(const std::string& path, const CsvOptions& options = {});

// Same, from an open stream. Used by the tests and handy for examples.
DataFrame read_csv(std::istream& in, const CsvOptions& options = {});

// Same, from text already in memory.
DataFrame parse_csv(const std::string& text, const CsvOptions& options = {});

}  // namespace vizlib
