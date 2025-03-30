# ComputerGraphics Exercise Project
## Required Software

- [CMake](https://cmake.org/)
- [Visual Studio (Windows)](https://visualstudio.microsoft.com/de/vs/community/)

## how to install
1. Download zip and unzip it
2. configure the project using cmake GUI or CLI <br>
    `cmake -B ./build`
3. (if it fails on linux you might need to install a view development packages)
4. Build cmake project using Visual Studio or CLI <br>
    `cmake --build ./build --config Debug`

program was tested on Windows (and in a previous Version on Linux). If you encounter any difficulties contact the lectuerer.

## linux setup guide:

1. install cmake<br>
  `sudo apt update`<br>
  `sudo apt install cmake pkg-config`
1. install build essential:<br>
  `sudo apt-get update && sudo apt-get install build-essential`
1. install git:<br>
  `sudo apt-get install git-all`
1. install opengl requirements:<br>
  `sudo apt-get install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev`<br>
  `sudo apt-get install libglew-dev libglfw3-dev libglm-dev`<br>
  `sudo apt-get install libao-dev libmpg123-dev`
1. install wayland (on Ubuntu [on other distributions you may need something else](https://www.glfw.org/docs/3.3/compile.html)):<br>
  `sudo apt install libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules`
1. configure cmake:<br>
  `cmake -B build`
1. build cmake:<br>
  `cmake --build ./build --config Debug` or `cmake --build ./build --config Debug --target compatibilityCheck`

if you want to use X11 instead of Wayland you need to change the [CmakeLists.txt in the imported folder](./imported/CMakeLists.txt):

set(GLFW_BUILD_X11 OFF)<br>
set(GLFW_BUILD_WAYLAND OFF)

to

set(GLFW_BUILD_X11 ON)<br>
set(GLFW_BUILD_WAYLAND OFF)

or

set(GLFW_BUILD_X11 OFF)<br>
set(GLFW_BUILD_WAYLAND OFF)

## Apple Notes
### Apple Silicon
I could not test Apple/Mac beforehand, so tweaking still needs to be done.
There a different methods to get the program running, but all require a bit of CMake knowledge.
- [mesa3d](https://www.mesa3d.org/)
- [MLG (OpenGL 4.6 on Metal)](https://github.com/openglonmetal/MGL)

If getting openGL with compute Shaders (4.3) to run on Mac OS and alternative options for Mac users are not given, i will cut compute shaders from the Topics.
And i will downgrade the sample project to 3.3. which should run all Macs (no guarantees).

### Non-Apple Silicon
The safest route is a linux or windows dual boot. Alternatively the Apple Silicon methods can be tried.

### Why OpenGL if Mac dose not support if fully?
Linux and Windows support it. And the only cross platform option is Vulkan which is out of scope for an entry level Computer Graphics course.