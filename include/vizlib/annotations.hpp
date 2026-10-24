
#pragma once

#include <string>
#include <vector>

#include "vizlib/color.hpp"
#include "vizlib/geometry.hpp"

namespace vizlib {

class Canvas;
class TextRenderer;

struct Annotation {
    std::string text;
    Point position;
    double fontSize = 12.0;
    Color color{0, 0, 0};
};

class Annotations {
public:
    void add_text(
        const std::string& text,
        Point position,
        double fontSize = 12.0,
        Color color = Color{0, 0, 0}
    );

    const std::vector<Annotation>& get_annotations() const;

    // Draw all stored annotations onto the canvas.
    void draw(
        Canvas& canvas,
        const TextRenderer& renderer
    ) const;

    void clear();

private:
    std::vector<Annotation> annotations_;
};

} // namespace vizlib
