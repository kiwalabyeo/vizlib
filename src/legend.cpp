
#include "vizlib/legend.hpp"

#include "vizlib/canvas.hpp"
#include "vizlib/text.hpp"

#include <stdexcept>

namespace vizlib {

void Legend::add_entry(const std::string& label, Color color) {
    entries_.push_back(LegendEntry{label, color});
}

const std::vector<LegendEntry>& Legend::get_entries() const {
    return entries_;
}

void Legend::clear() {
    entries_.clear();
}

void Legend::set_position(Point position) {
    position_ = position;
}

Point Legend::get_position() const {
    return position_;
}

// Implements the Drawable interface.
void Legend::draw(
    Canvas& canvas,
    const TextRenderer& renderer
) const {
    draw(canvas, renderer, 12.0);
}

// Draws the legend using the specified font size.
void Legend::draw(
    Canvas& canvas,
    const TextRenderer& renderer,
    double fontSize
) const {
    if (fontSize <= 0.0) {
        throw std::invalid_argument(
            "Font size must be positive."
        );
    }

    const double swatchSize = 12.0;
    const double textGap = 6.0;
    const double rowGap = 6.0;
    const double rowHeight =
        (fontSize > swatchSize ? fontSize : swatchSize) + rowGap;

    for (std::size_t i = 0; i < entries_.size(); ++i) {
        const auto& entry = entries_[i];
        const double y = position_.y + i * rowHeight;

        // Draw the colored square for this entry.
        canvas.fill_rect(
            Rect{
                position_.x,
                y,
                swatchSize,
                swatchSize
            },
            entry.color
        );

        // Draw the label beside the square.
        renderer.draw_text(
            canvas,
            entry.label,
            Point{
                position_.x + swatchSize + textGap,
                y + fontSize
            },
            fontSize,
            Color{0, 0, 0}
        );
    }
}

} // namespace vizlib
