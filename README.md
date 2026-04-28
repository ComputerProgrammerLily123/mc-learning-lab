# mc-learning-lab

> A learning project: building a Minecraft-style voxel game with OpenGL.

This repository documents my journey of learning game development and OpenGL by recreating Minecraft's core mechanics from scratch. **Expect messy code, experiments, and things that don't work.**

## 🎯 Goals

- [x] Render a single cube with texture
- [x] First-person camera (WASD + mouse)
- [x] Chunk system (16×256×16)
- [x] Basic terrain generation (Perlin noise)
- [x] Block placing / breaking
- [ ] Save & load world
- [ ] more...

## 🛠️ Tech Stack

| Component | Choice |
|-----------|--------|
| Language | C++11 |
| Graphics | OpenGL 3.3 Core |
| Window | GLFW |
| Math | GLM |
| Texture | stb_image |

## External Assets
- stb_image (https://github.com/nothings/stb)
- FastNoiseLite (https://github.com/Auburn/FastNoiseLite)

## 🚀 Getting Started

### Prerequisites

- CMake 3.10+
- C++ compiler with C++11 support
- OpenGL 3.3+ compatible GPU

### Textures

#### Resource Files Notice
Due to copyright reasons, this repository does not include the original Minecraft textures.

#### How to Obtain Textures
- Purchase the official version of Minecraft

- Extract them from the .minecraft/assets/ directory

- Or use your own custom texture pack

After obtaining the textures, place the textures folder from the texture pack into the same directory as the executable file.

### Build

```bash
git clone https://github.com/[your-name]/mc-learning-lab.git
cd mc-learning-lab
mkdir build && cd build
cmake ..
make
./mc-learning-lab
