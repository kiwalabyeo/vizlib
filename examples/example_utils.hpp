// Small helper shared by the examples. Not part of the library.
#pragma once

#include <vizlib/dataframe.hpp>

#include <cmath>
#include <string>
#include <variant>
#include <vector>

namespace examples {

// Copies one column of a DataFrame into plain numbers, ready for plotting.
// Cells that are not numbers become NaN, which the plots skip.
inline std::vector<double> numeric_column(const vizlib::DataFrame& df, const std::string& name)
{
    std::vector<double> values;
    for (const vizlib::CellValue& cell : df.get_column(name))
    {
        const double* number = std::get_if<double>(&cell);
        values.push_back(number ? *number : NAN);
    }
    return values;
}

}  // end of namespace examples
