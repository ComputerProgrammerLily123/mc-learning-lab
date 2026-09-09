#pragma once
#include <memory>
#include "texture.h"
class TextureRegion
{
public:
    TextureRegion() = default;
    explicit TextureRegion(std::shared_ptr<Texture> texture);
    TextureRegion(std::shared_ptr<Texture> texture, unsigned px1, unsigned py1, unsigned px2, unsigned py2, unsigned atlasWidth, unsigned atlasHeight);

    [[nodiscard]] std::shared_ptr<Texture> GetTexturePtr() const { return texturePtr; };
    [[nodiscard]] float GetU1() const { return u1; };
    [[nodiscard]] float GetV1() const { return v1; };
    [[nodiscard]] float GetU2() const { return u2; };
    [[nodiscard]] float GetV2() const { return v2; };
    [[nodiscard]] int GetWidth() const { return width; };
    [[nodiscard]] int GetHeight() const { return height; };

    void Bind(unsigned slot = 0) const;
private:
    std::shared_ptr<Texture> texturePtr;
    float u1 {0.0f}, v1 {0.0f}, u2 {1.0f}, v2 {1.0f};
    int width {0}, height {0};
};