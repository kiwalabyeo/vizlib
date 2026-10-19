
#pragma once

#include <string>
#include <vector>

#include "vizlib/color.hpp"
#include "vizlib/geometry.hpp"

namespace vizlib {

struct LegendEntry {
    std::string label;
    Color color{0, 0, 0};
};

class Legend {
public:
    void add_entry(const std::string& label, Color color);

    const std::vector<LegendEntry>& get_entries() const;

    void clear();

    void set_position(Point position);
    Point get_position() const;

private:
    std::vector<LegendEntry> entries_;
    Point position_{10.0, 10.0};
};

} // namespace vizlib
