#pragma once
#include <array>
struct GlyphMetrics
{
    unsigned left;
    unsigned width;
    unsigned advance;
};
class BitmapFont
{
public:
    BitmapFont(const unsigned char* pixels, int atlasWidth, int atlasHeight, int channels, bool flipped);
    [[nodiscard]] const GlyphMetrics& GetGlyph(unsigned char c) const;
private:
    std::array<GlyphMetrics, 256> glyphs {};
    [[nodiscard]] GlyphMetrics InitGlyph(const unsigned char* pixels, int atlasWidth, int atlasHeight, int channels, unsigned code, bool flip) const;
};