# C++ PoC mesh renderer
Proof-of-concept OBJ GPU-accelerated renderer on top of OpenGL API and TinyObjLoader.
My pipline include few steps which are written bellow.

## What I have done:
- **Shaders** - Vertex + Fragment with Phong
- **Rusterization** - GPU accelerated (OpenGL 3.3 Core)
- **FPS Camera**
- **Matrices of view** - model + view + GLM projection
- **OBJ load** - TinyObjLoader (v/vt/vn)
- **Texturing** - STB_image based
- **UV interpolation**

## How to build
### The only way now is xmake.
```bash
git clone https://github.com/byteonabeach/obj-renderer-poc.git
cd obj-renderer-poc
xmake
xmake run
```

## Control
- **WASD** - movement
- **Mouse** - view
- **Mouse wheel** - zoom (FOV)

## Dependencies (prefer xmake over manual installation)
- OpenGL 3.3+
- GLFW
- GLAD
- GLM
- STB_image
- TinyObjLoader
