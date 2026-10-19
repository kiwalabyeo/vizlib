#include "vizlib/text.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace vizlib {

struct TextRenderer::Impl {
    std::string fontBuffer;
    stbtt_fontinfo fontInfo{};
    bool fontLoaded = false;
};

TextRenderer::TextRenderer(const std::filesystem::path& fontPath)
    : impl_(std::make_unique<Impl>()) {

    std::ifstream fontFile(fontPath, std::ios::binary);

    if (!fontFile) {
        throw std::runtime_error("Could not open font file");
    }

    impl_->fontBuffer.assign(
        std::istreambuf_iterator<char>(fontFile),
        std::istreambuf_iterator<char>()
    );

    if (impl_->fontBuffer.empty()) {
        throw std::runtime_error("Font file is empty");
    }

    const auto* fontData =
        reinterpret_cast<const unsigned char*>(
            impl_->fontBuffer.data()
        );

    int offset = stbtt_GetFontOffsetForIndex(fontData, 0);

    if (offset < 0 ||
        !stbtt_InitFont(&impl_->fontInfo, fontData, offset)) {
        throw std::runtime_error("Could not load font");
    }

    impl_->fontLoaded = true;
}

TextRenderer::~TextRenderer() = default;

TextRenderer::TextRenderer(TextRenderer&&) noexcept = default;

TextRenderer& TextRenderer::operator=(TextRenderer&&) noexcept = default;

TextMetrics TextRenderer::measure_text(
    std::string_view text,
    double fontSize
) const {
    if (!impl_ || !impl_->fontLoaded) {
        throw std::runtime_error("Font is not loaded");
    }

    if (fontSize <= 0.0) {
        throw std::invalid_argument("Font size must be positive");
    }

    float scale = stbtt_ScaleForPixelHeight(
        &impl_->fontInfo,
        static_cast<float>(fontSize)
    );

    float width = 0.0f;

    for (std::size_t i = 0; i < text.size(); ++i) {
        int advanceWidth = 0;
        int leftSideBearing = 0;

        unsigned char character =
            static_cast<unsigned char>(text[i]);

        stbtt_GetCodepointHMetrics(
            &impl_->fontInfo,
            character,
            &advanceWidth,
            &leftSideBearing
        );

        width += advanceWidth * scale;

        if (i + 1 < text.size()) {
            width += stbtt_GetCodepointKernAdvance(
                &impl_->fontInfo,
                character,
                static_cast<unsigned char>(text[i + 1])
            ) * scale;
        }
    }

    int ascent = 0;
    int descent = 0;
    int lineGap = 0;

    stbtt_GetFontVMetrics(
        &impl_->fontInfo,
        &ascent,
        &descent,
        &lineGap
    );

    TextMetrics metrics;
    metrics.width = width;
    metrics.height = (ascent - descent) * scale;

    return metrics;
}

void TextRenderer::draw_text(
    Canvas& canvas,
    std::string_view text,
    Point position,
    double fontSize,
    Color color
) const {
    if (!impl_ || !impl_->fontLoaded) {
        throw std::runtime_error("Font is not loaded");
    }

    if (fontSize <= 0.0) {
        throw std::invalid_argument("Font size must be positive");
    }

    float scale = stbtt_ScaleForPixelHeight(
        &impl_->fontInfo,
        static_cast<float>(fontSize)
    );

    float x = static_cast<float>(position.x);
    float baseline = static_cast<float>(position.y);

    for (std::size_t i = 0; i < text.size(); ++i) {
        unsigned char character =
            static_cast<unsigned char>(text[i]);

        int advanceWidth = 0;
        int leftSideBearing = 0;
        int x0 = 0;
        int y0 = 0;
        int x1 = 0;
        int y1 = 0;

        stbtt_GetCodepointHMetrics(
            &impl_->fontInfo,
            character,
            &advanceWidth,
            &leftSideBearing
        );

        stbtt_GetCodepointBitmapBox(
            &impl_->fontInfo,
            character,
            scale,
            scale,
            &x0,
            &y0,
            &x1,
            &y1
        );

        int bitmapWidth = 0;
        int bitmapHeight = 0;

        unsigned char* bitmap = stbtt_GetCodepointBitmap(
            &impl_->fontInfo,
            scale,
            scale,
            character,
            &bitmapWidth,
            &bitmapHeight,
            nullptr,
            nullptr
        );

        if (bitmap != nullptr) {
            for (int by = 0; by < bitmapHeight; ++by) {
                for (int bx = 0; bx < bitmapWidth; ++bx) {
                    int pixelX = static_cast<int>(
                        std::round(x + x0 + bx)
                    );

                    int pixelY = static_cast<int>(
                        std::round(baseline + y0 + by)
                    );

                    if (pixelX < 0 || pixelX >= canvas.width() ||
                        pixelY < 0 || pixelY >= canvas.height()) {
                        continue;
                    }

                    unsigned char coverage =
                        bitmap[by * bitmapWidth + bx];

                    if (coverage == 0) {
                        continue;
                    }

                    Color background =
                        canvas.get_pixel(pixelX, pixelY);

                    float alpha = coverage / 255.0f;

                    Color blended = color;

                    blended.r = static_cast<unsigned char>(
                        color.r * alpha +
                        background.r * (1.0f - alpha)
                    );

                    blended.g = static_cast<unsigned char>(
                        color.g * alpha +
                        background.g * (1.0f - alpha)
                    );

                    blended.b = static_cast<unsigned char>(
                        color.b * alpha +
                        background.b * (1.0f - alpha)
                    );

                    canvas.set_pixel(pixelX, pixelY, blended);
                }
            }

            stbtt_FreeBitmap(bitmap, nullptr);
        }

        x += advanceWidth * scale;

        if (i + 1 < text.size()) {
            x += stbtt_GetCodepointKernAdvance(
                &impl_->fontInfo,
                character,
                static_cast<unsigned char>(text[i + 1])
            ) * scale;
        }
    }
}

} // namespace vizlib

