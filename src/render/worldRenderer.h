#pragma once
#include "shader.h"
#include <vector>
#include <unordered_map>
#include "core/sceneManager.h"
class Camera;
class World;
class WorldRenderer : public SceneObject
{
public:
    WorldRenderer(const WorldRenderer&) = delete;
    WorldRenderer(WorldRenderer&&) = delete;
    WorldRenderer& operator=(const WorldRenderer&) = delete;
    WorldRenderer& operator=(WorldRenderer&&) = delete;
    WorldRenderer(unsigned size);
    ~WorldRenderer();
    void Clear();
    void Draw(World *world, Camera *camera, int32_t centralX, int32_t centralZ);

private:

    static constexpr glm::vec4 skyColor = {0.53f, 0.81f, 0.92f, 1.0f};

    int renderRadian;
    unsigned VAO = 0;

    std::unordered_map<uint64_t, unsigned> VBO, EBO;
    Shader shader = Shader("shaders/vertex.vert", "shaders/fragment.frag");

    void DrawMesh(int32_t chunkX, int32_t chunkZ, const std::vector<float> &vertices, const std::vector<unsigned> &indices, bool &isDirty);
    bool IsChunkVisible(int32_t chunkX, int32_t chunkZ, const std::array<glm::vec4, 6>& planes);
};