# TinyRenderer

This repository contains a minimal software renderer written in C++. It can load Wavefront OBJ models and render them into TGA images using a simple Phong shading pipeline.

## Building

A C++11 compiler is required. To build the project, run:

```sh
g++ -std=c++11 main.cpp model.cpp our_gl.cpp tgaimage.cpp matrix\&vector_math.cpp -o main
```

## Running

After building, execute `./main` to render the default model or provide a path to an OBJ file as an argument:

```sh
./main path/to/model.obj
```

The renderer outputs `output.tga` with the final image and `zbuffer.tga` containing depth values.

## Project Structure

- `main.cpp` – sets up the scene and runs the rendering loop
- `model.cpp/h` – OBJ model loading and texture handling
- `matrix&vector_math.*` – simple vector and matrix math utilities
- `our_gl.*` – rasterization and shader interfaces
- `tgaimage.*` – TGA image read/write utilities

This implementation is a lightweight version of the renderer described in the [Tinyrenderer](https://github.com/ssloy/tinyrenderer) tutorial.
