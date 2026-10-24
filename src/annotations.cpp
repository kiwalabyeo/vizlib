
#include "vizlib/annotations.hpp"

#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

#include <stdexcept>
#include <utility>

namespace vizlib {

void Annotations::add_text(
    const std::string& text,
    Point position,
    double fontSize,
    Color color
) {
    if (fontSize <= 0.0) {
        throw std::invalid_argument(
            "Font size must be positive."
        );
    }

    annotations_.push_back(
        Annotation{text, position, fontSize, color}
    );
}

const std::vector<Annotation>&
Annotations::get_annotations() const {
    return annotations_;
}

void Annotations::draw(
    Canvas& canvas,
    const TextRenderer& renderer
) const {
    for (const auto& annotation : annotations_) {
        renderer.draw_text(
            canvas,
            annotation.text,
            annotation.position,
            annotation.fontSize,
            annotation.color
        );
    }
}

void Annotations::clear() {
    annotations_.clear();
}

} // namespace vizlib
