# 3D Viewer
This is my implementation of the 3D Viewer application for the 'Computer Graphics' course at Umeå University.
The implementation is done in C++ using OpenGL and GLSL.

## Building
The project is built using CMake. It can be built on both Windows and Linux.
To build, run the following commands:
```bash
mkdir build
cd build
cmake ..
make
```

## Usage
To run the application, run the following command:
```bash
./3DViewer
```

## Controls
The entire application is controlled using the mouse and keyboard, in combination with the GUI.
### Mouse Controls
- **Hold Left Mouse Button**: Rotate the camera.
- **Scroll**: Zoom in and out.
### Keyboard Controls
- **W**: Move the camera forward.
- **A**: Move the camera left.
- **S**: Move the camera backward.
- **D**: Move the camera right.
- **E/Space**: Move the camera up.
- **Q/Left Shift**: Move the camera down.
- **J**: Move the currently selected object to the negative X direction.
- **L**: Move the currently selected object to the positive X direction.
- **I**: Move the currently selected object to the positive Y direction.
- **K**: Move the currently selected object to the negative Y direction.
- **Up Arrow**: Move the currently selected object counter-clockwise around the Y axis.
- **Down Arrow**: Move the currently selected object clockwise around the Y axis.
- **Left Arrow**: Move the currently selected object clockwise around the X axis.
- **Right Arrow**: Move the currently selected object counter-clockwise around the X axis.
- **Esc**: Exit the application.
- ### GUI Controls
- **> Scene**: Load new scenes by selecting an object file.
- **> Models**: Look at all the loaded models in the scene.
Modify material groups of the model.
Select the model that is manipulated by keyboard controls.
- **> Projection**: Change the projection type and specifics of the camera.
- **> Ambient Light**: Change the ambient light color and intensity.
- **> Light**: Change the light color and intensity.
- **> Camera**: View camera information and modify camera speed.
- **> Ground floor**: Modify the visibility of the ground floor. (The color can be modified in the 'Models' section)
- **> Solar System Settings**: Modify the simulation speed for the 'Solar System' example scene.

## Bonus Tasks
I have completed the following bonus tasks:
- 1 point: Add a ground floor.
- 1 point: Add a Skybox.
- 1 point: Read and use the texture coordinates from the object file.
- 1 point: Read and use MTL file.
- 2 points: Add several objects to the scene using compound objects (multiple defined objects in a single .obj file).

## Libraries used
The project uses the following libraries (that are all included in the 'lib' folder):
- [GLEW v2.1.0](http://glew.sourceforge.net/)
- [GLFW v3.3.8](https://www.glfw.org/)
- [GLM v0.9.9.8](https://glm.g-truc.net/0.9.9/index.html)
- [ImGui v1.90](https://github.com/ocornut/imgui)
- [ImGuiFileDialog v0.6.6.1](https://github.com/aiekick/ImGuiFileDialog)
- [STB Image v2.29](https://github.com/nothings/stb/blob/master/stb_image.h)
- [Tiny OBJ Loader v2.0](https://github.com/tinyobjloader/tinyobjloader)