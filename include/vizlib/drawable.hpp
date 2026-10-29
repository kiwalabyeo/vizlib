
#pragma once

namespace vizlib {

class Canvas;
class TextRenderer;

// Common interface for objects that can be drawn on a canvas.
class Drawable {
public:
    virtual ~Drawable() = default;

    virtual void draw(
        Canvas& canvas,
        const TextRenderer& renderer
    ) const = 0;
};

} // namespace vizlib
