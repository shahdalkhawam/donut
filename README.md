# Donut

A small OpenGL/FreeGLUT donut demo.

## Build

From the project root:

```sh
g++ main.cpp -o donut.exe -Iinclude -Llib -lfreeglut -lopengl32 -lglu32
```

The project includes the required FreeGLUT headers and libraries in the local `include/` and `lib/` folders.

## Run

```sh
donut.exe
```
