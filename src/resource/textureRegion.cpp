#include "textureRegion.h"
TextureRegion::TextureRegion(std::shared_ptr<Texture> texture) : texturePtr(std::move(texture)) {
    if (texturePtr) {
        width = texturePtr->GetWidth();
        height = texturePtr->GetHeight();
    }
}
TextureRegion::TextureRegion(std::shared_ptr<Texture> texture, unsigned px1, unsigned py1, unsigned px2, unsigned py2, unsigned atlasWidth, unsigned atlasHeight) : texturePtr(std::move(texture)), u1(static_cast<float>(px1) / atlasWidth), v1(static_cast<float>(py1) / atlasHeight), u2(static_cast<float>(px2) / atlasWidth), v2(static_cast<float>(py2) / atlasHeight), width(width), height(height) 
{ 
}
void TextureRegion::Bind(unsigned slot) const
{
    if(texturePtr)
    {
        texturePtr->Bind(slot);
    }
}