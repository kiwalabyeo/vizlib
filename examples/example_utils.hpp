// Small helpers shared by the examples. Not part of the library.
#pragma once

#include <vizlib/canvas.hpp>
#include <vizlib/image_export.hpp>

#include <cstdint>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace examples {

// Reads one numeric column of a simple CSV file (no quoted fields), by its
// header name. Throws std::runtime_error if the file or column is missing or a
// value is not a number.
//
// Temporary: to be replaced by Member 1's vizlib::read_csv() once it is merged.
inline std::vector<double> read_csv_column(const std::string& path, const std::string& column)
{
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Could not open " + path + " (run the example from the project root)");

    auto split = [](const std::string& line) {
        std::vector<std::string> cells;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ','))
            cells.push_back(cell);
        return cells;
    };

    std::string line;
    std::getline(file, line);
    const std::vector<std::string> header = split(line);
    std::size_t index = header.size();
    for (std::size_t i = 0; i < header.size(); ++i)
        if (header[i] == column)
            index = i;
    if (index == header.size())
        throw std::runtime_error("No column '" + column + "' in " + path);

    std::vector<double> values;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        const std::vector<std::string> cells = split(line);
        if (index >= cells.size())
            throw std::runtime_error("Short row in " + path + ": " + line);
        try
        {
            values.push_back(std::stod(cells[index]));
        }
        catch (const std::exception&)
        {
            throw std::runtime_error("Not a number in " + path + ": " + cells[index]);
        }
    }
    return values;
}

// Saves a Canvas as a PNG. Returns false if the file could not be written.
//
// Temporary: to be replaced by Member 7's canvas export adapter once it is merged.
inline bool save_canvas_png(const vizlib::Canvas& canvas, const char* path)
{
    std::vector<std::uint8_t> bytes;
    bytes.reserve(static_cast<std::size_t>(canvas.width()) * canvas.height() * 4);
    for (int row = 0; row < canvas.height(); ++row)
    {
        for (int col = 0; col < canvas.width(); ++col)
        {
            const vizlib::Color c = canvas.get_pixel(col, row);
            bytes.insert(bytes.end(), {c.r, c.g, c.b, c.a});
        }
    }
    return vizlib::save_png(path, canvas.width(), canvas.height(), 4, bytes.data());
}

}  // end of namespace examples
