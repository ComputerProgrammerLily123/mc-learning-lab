#ifndef TEXTURE_H
#define TEXTURE_H

#define ATLAS_SIZE 32

#include <string>

class Texture{
public:
    Texture(const std::string& path);
    ~Texture();
    void Bind(unsigned slot = 0) const;
    void Unbind() const;
    void CreateAtlas();
    void LoadTexture(const std::string &filename);
    private:
    std::string loadPath; 
    unsigned ID;
    void LoadTextureToAtlas(const std::string& filename, int x, int y, int width, int heigh);
};
#endif