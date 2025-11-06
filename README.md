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


## Time for qoutes!
**I'm C++23... The latest and greatest!**

*stares intensely at reflection in terminal window*

**I bring you modules, concepts, ranges, coroutines, and all the modern features you've been waiting for...**

*flexes with std::format*

**I'm the future of programming... Everybody loves me!**

*adjusts glasses with a smug look*

**GCC? Oh, GCC... My dear, sweet GCC...**

*chuckles condescendingly*

**GCC modules? Haha, that's a good one! GCC says "I support C++23 modules!" but then crashes and burns like a failed compilation with template errors...**

*waves dismissively*

**"I'm a C++23 compiler... Modules work perfectly!" says GCC, right before segfaulting on std::integral_constant...**

*mocking impression voice*

**"Oh look, I can compile your header units... But I'll just crash when you try to use them!"**

*bursts into laughter*

**Meanwhile, I'm over here with my std::span, std::ranges, and std::expected working perfectly...**

*strikes a heroic pose*

**I'm C++23... The real MVP... Modules will work eventually... Maybe in GCC 16.0... Or 17.0...**

*winks at camera*

**I'm still C++23... And I'm still the future... Even if GCC modules are stuck in 2020...**

*flexes again*

**Remember kids: Use modules when they work, fall back to headers when they don't... That's what C++23 is all about!**
