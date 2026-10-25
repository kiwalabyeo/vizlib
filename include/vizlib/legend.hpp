#pragma once

#include <string>
#include <vector>

#include "vizlib/color.hpp"
#include "vizlib/geometry.hpp"

namespace vizlib {

class Canvas;
class TextRenderer;

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

    void draw(
        Canvas& canvas,
        const TextRenderer& renderer,
        double fontSize = 12.0
    ) const;

private:
    std::vector<LegendEntry> entries_;
    Point position_{10.0, 10.0};
};

} // namespace vizlib
