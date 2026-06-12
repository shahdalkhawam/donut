# Donut

A small OpenGL/FreeGLUT demo that renders a spinning torus (a donut-shaped 3D surface) with three display modes: points, lines, and a shadow-style ASCII view.

<p align="left">
<img width="350" height="250" alt="image" src="https://github.com/user-attachments/assets/5af8bfb5-119b-4a8f-8e77-c8025563faa3" />
<img width="350" height="250" alt="image" src="https://github.com/user-attachments/assets/b9a5060b-d85d-450f-88ad-f58138569db2" />
<img width="350" height="250" alt="image" src="https://github.com/user-attachments/assets/f5140586-b0f7-4f4f-b79a-9563db5a8dab" />
<img width="350" height="250" alt="image" src="https://github.com/user-attachments/assets/e621dd2c-e8f8-4973-987a-121a9c43888e" />


</p>


The program draws a parametrically defined 3D torus in 3D rotating in real time using OpenGL transforms. The math is based on the standard torus equations:

$$
x = (R + r \cos \theta) \cos \phi
$$

$$
y = r \sin \theta
$$

$$
z = (R + r \cos \theta) \sin \phi
$$

- $R$ is the major radius (distance from the center of the tube to the center of the torus) -In this implementation $R = 0.75$
- $r$ is the minor radius (radius of the tube itself)-In this implementation $r = 0.25$ 
- $\theta$ sweeps around the tube cross-section
- $\phi$ sweeps around the donut ring

The code samples $\theta$ and $\phi$ in small increments, creating a dense set of points on the torus surface.

## The 3D math and rendering logic

### 1. Parametric torus generation

Inside the display loop, the program computes a vertex for each pair of angles:

- `theta` runs from $0$ to $2\pi$
- `phi` runs from $0$ to $2\pi$
- each step is about $0.08$ radians

This produces a mesh of points or line segments that approximate the torus.

### 2. Rotation and camera transform

Before drawing, the code updates the model-view matrix:

1. `glLoadIdentity()` resets the current transform.
2. `glTranslatef(0.0f, 0.0f, zoom)` moves the object toward or away from the camera.
3. `glRotatef(xRotation, 1.0f, 0.0f, 0.0f)` tilts the object around the X axis.
4. `glRotatef(yRotation + angle, 0.0f, 1.0f, 0.0f)` rotates it around the Y axis.

The `angle` value increases over time, so the donut appears to spin continuously.

### 3. Projection setup

The `reshape()` function sets up the perspective view:

- `gluPerspective(45.0f, aspectRatio, 1.0f, 100.0f)`
- this gives the scene a standard 3D camera perspective

### 4. Display modes

The program supports three modes:

- `MODE_POINTS` — draws the torus as a cloud of points
- `MODE_LINES` — draws connected line segments
- `MODE_SHADOW` — uses projected ASCII-style glyphs to simulate a shadowed version of the torus

The shadow mode uses `gluProject()` to convert 3D coordinates into 2D screen-space positions. This allows the code to place characters on the display as if they were projected onto the screen.

### 5. ASCII shadow effect

The shadow mode uses a simple depth-based character map:

```text
" .:-=+*#%@"
```

A function computes a depth value from the absolute coordinates and maps it to one of the characters. The result gives the illusion of a darker or lighter surface based on how far the point is from the viewer’s perspective.

## Controls

Keyboard:
- `P` — point mode
- `L` — line mode
- `H` — shadow mode
- `W / S` — tilt the object up / down
- `A / D` — rotate left / right
- `+ / -` — increase / decrease torus size
- `Z / X` — zoom in / out
- `R` — reset all parameters
- `Esc` — exit

Special keys:
- `Up / Down` — increase / decrease rotation speed
- `Left / Right` — adjust the major torus radius

## BUILD & RUN

From the project root:
Build with this command creating a runnable donut.exe :

```sh
g++ main.cpp -o donut.exe -Iinclude -Llib -lfreeglut -lopengl32 -lglu32
```

The project uses the local FreeGLUT headers and libraries in `include/` and `lib/`.
Run the donut.exe file:
```sh
donut.exe
```

## Summary

This program combines:
- parametric geometry for the torus
- real-time rotation and projection matrices
- simple lighting/shadow-style stylization
- interactive controls for experimentation

It is a compact example of how a mathematically defined 3D surface can be visualized with OpenGL and FreeGLUT.
