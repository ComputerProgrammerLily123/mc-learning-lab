#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H
#include "shader.h"
#include "texture.h"
#include <vector>
#include <unordered_map>
class Camera;
class World;
class WorldRenderer
{
public:
    WorldRenderer(unsigned size);
    ~WorldRenderer();
    void Clear();
    void Draw(World *world, Camera *camera, int32_t centralX, int32_t centralZ);

private:
    int32_t renderRadian;
    unsigned VAO;

    std::unordered_map<uint64_t, unsigned> VBO, EBO;
    Shader shader = Shader("shaders/vertex.vert", "shaders/fragment.frag");
    Texture texture = Texture("textures/blocks");

    void DrawMesh(int32_t chunkX, int32_t chunkZ, const std::vector<float> &vertices, const std::vector<unsigned> &indices, bool &isDirty);
    bool IsChunkVisible(int32_t chunkX, int32_t chunkZ, const glm::mat4 &vp);
};
#endif