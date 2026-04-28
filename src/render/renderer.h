#ifndef RENDERER_H
#define RENDERER_H
class Camera;
class World;

#include "shader.h"
#include "texture.h"
class Renderer{
public:
    Renderer(unsigned size);
    ~Renderer();
    void Clear();
    void Draw(World* world,Camera* camera);
private:
    unsigned worldSize;
    Shader shader;
    Texture texture;
};
#endif