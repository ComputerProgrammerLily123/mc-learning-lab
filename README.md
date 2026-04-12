# mc-learning-lab

> A learning project: building a Minecraft-style voxel game with OpenGL.

This repository documents my journey of learning game development and OpenGL by recreating Minecraft's core mechanics from scratch. **Expect messy code, experiments, and things that don't work.**

## 🎯 Goals

- [ ] Render a single cube with texture
- [ ] First-person camera (WASD + mouse)
- [ ] Chunk system (16×256×16)
- [ ] Basic terrain generation (Perlin noise)
- [ ] Block placing / breaking
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

## 🚀 Getting Started

### Prerequisites

- CMake 3.10+
- C++ compiler with C++11 support
- OpenGL 3.3+ compatible GPU

### Build

```bash
git clone https://github.com/[your-name]/mc-learning-lab.git
cd mc-learning-lab
mkdir build && cd build
cmake ..
make
./mc-learning-lab
