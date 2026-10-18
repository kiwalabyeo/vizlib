
#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

#include "vizlib/canvas.hpp"
#include "vizlib/color.hpp"
#include "vizlib/geometry.hpp"

namespace vizlib {

struct TextMetrics {
    double width = 0.0;
    double height = 0.0;
};

class TextRenderer {
public:
    explicit TextRenderer(const std::filesystem::path& fontPath);

    ~TextRenderer();

    TextRenderer(TextRenderer&&) noexcept;
    TextRenderer& operator=(TextRenderer&&) noexcept;

    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;

    TextMetrics measure_text(
        std::string_view text,
        double fontSize
    ) const;

    void draw_text(
        Canvas& canvas,
        std::string_view text,
        Point position,
        double fontSize,
        Color color
    ) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace vizlib
