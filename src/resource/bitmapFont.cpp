#include "bitmapFont.h"
BitmapFont::BitmapFont(const unsigned char* pixels, int atlasWidth, int atlasHeight, int channels, bool flipped)
{
    for (unsigned code = 0; code < glyphs.size(); ++code)
    {
        glyphs[code] = InitGlyph(pixels, atlasWidth, atlasHeight, channels, code, flipped);
    }
}
const GlyphMetrics& BitmapFont::GetGlyph(unsigned char c) const
{
    return glyphs.at(c);
}
GlyphMetrics BitmapFont::InitGlyph(const unsigned char* pixels, int atlasWidth, int atlasHeight, int channels, unsigned code, bool flip) const
{
    constexpr int cellSize = 8;
    constexpr int columns = 16;

    const int logicalRow = code / columns;

    const int cellX = static_cast<int>(code % columns) * cellSize;

    const int cellY = flip ? atlasHeight - (logicalRow + 1) * cellSize : logicalRow * cellSize;

    int minX = cellSize;
    int maxX = -1;

    for (int y = 0; y < cellSize; ++y)
    {
        for (int x = 0; x < cellSize; ++x)
        {
            const std::size_t index = (std::size_t(cellY + y) * atlasWidth + std::size_t(cellX + x)) * channels;

            const unsigned char alpha = channels >= 4 ? pixels[index + 3] : pixels[index];

            if (alpha > 16)
            {
                minX = std::min(minX, x);
                maxX = std::max(maxX, x);
            }
        }
    }

    if (maxX < minX)
    {
        return GlyphMetrics{
            .left = 0,
            .width = 0,
            .advance = 4
        };
    }

    const auto width = static_cast<unsigned>(maxX - minX + 1);

    return GlyphMetrics{
        .left = static_cast<unsigned>(minX),
        .width = width,
        .advance = width + 1
    };
}