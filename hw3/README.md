# HW3

Srihith Bharadwaj Burra

116728900

srihithbharadw.burra@stonybrook.edu
srburra@cs.stonybrook.edu

## Overview

Implemented a sample program to display flat-shaded triangle, tetrahedron and sphere (with tessellation shaders). 
Also implemented a FPS-style camera and local illumination with the Phong shading model. 

Note: Directory `./var/` contains vertices for the required polyhedral objects. 
Each line denotes a 3D point (x, y, and z coordinates), and each 3 lines denote a triangular facet. 
Note that many points are duplicated as they appear in multiple facets!

## Notes
- Go into the hw3/cpp folder and run ./build_script.sh command
- All README files for future homework should also comply with the same format as this one. 
- This program template is just for your reference. Please feel free to code your own program (i.e., not using this template). However, the user interface (mouse and keyboard functionalities) should be the same as specified in the homework manual. 
- Please submit either the C++ version or the Python version (but **not both**), and **comply with the submission requirements as detailed on the [TA Help Page](https://www3.cs.stonybrook.edu/~xihan1/courses/cse528/ta_help_page.html)**. Plesase cut and paste this README (together with your answers for the non-programming part) into either `cpp/` or `py/`, rename the directory as instructed by the TA Help Page, and submit via Brightspace. 
- Please also make sure you have checked all implemented features with "x"s in the Markdown table below. As speficied on the TA Help Page, only checked features will be considered for grading!

## Hints on The Template

- Suggested order to read and understand this program: 
  - GLFW callbacks;
  - Triangle (ignore the code related to the self-spin effect);
  - Triangle (with self-spin; involves transformation matrices);
  - Circles (involves tessellation shaders, which are not necessary in the first half of this course). 
- In this program, the circle parameters are passed into tessellation shaders via generic vertex attribute arrays. 
  Note how this differs from the "pass-by-shader-uniforms" method for the sphere example; 
- Please do remember to play with the program as guided by the comments in the tessellation evaluation shader;
- If this program does not work on your VMWare virtual environment, 
  please try to [disable the 3D acceleration feature](https://kb.vmware.com/s/article/59146). 

## Dependencies

- OpenGL (Required for Both Versions):
```bash
sudo add-apt-repository ppa:kisak/kisak-mesa
sudo apt update
sudo apt-get dist-upgrade
sudo reboot
```
- Further Needed for the C/C++ Version: 
  - [GLAD](https://glad.dav1d.de/)
    - Configuration w.r.t. results of `sudo glxinfo | grep "OpenGL`
    - Command `glxinfo` needs `mesa-utils`
  - Remaining dependencies could be installed via apt:
  ```bash
  apt install libopencv-dev libglm-dev libglew-dev libglfw3-dev mesa-utils libx11-dev libxi-dev libxrandr-dev
  ```


## Compile & Run

- C/C++ Version (Run inside `cpp/`): 
```bash
mkdir build
cd build
cmake -DMAKE_BUILD_TYPE=Release ..
make 
cd ..
./build/hw3
```



## Usage

- Press `W`/`S`/`A`/`D`/`UP`/`DOWN`, or drag/scroll the mouse to adjust the camera. 

## Notes

- In this program, the sphere parameters passed into tessellation shaders via shader uniforms. 
  Note how this differs from the "pass-by-vertex-attribute-array" method for the circle example; 
- If this program does not work on your VMWare virtual environment, 
  please try to [disable the 3D acceleration feature](https://kb.vmware.com/s/article/59146). 

## Features Implemented

Check all features implemented with "x" in "[ ]"s. 
Only features or parts checked here would be graded! 

- [x] **P0: Global Functionalities** (See each object for display modes)
  - [x] Camera Functionalities
    - [x] Show/hide x, y, z Axes
    - [x] `W`/`S`/`A`/`D`/`UP`/`DOWN` Functionalities
- [x] **P1: Simple Polyhedral Objects**
  - [x] Tetrahedron
    - [x] Wireframe
    - [x] Flat
    - [ ] Smooth
  - [x] Cube
    - [x] Wireframe
    - [x] Flat
    - [ ] Smooth
  - [x] Octahedron
    - [x] Wireframe
    - [x] Flat
    - [ ] Smooth
- [x] **P2: Icosahedron**
  - [x] Wireframe
  - [x] Flat
  - [ ] Smooth
  - [x] Subdivision
- [x] **P3: Ellipsoid**
  - [x] Wireframe
  - [x] Flat
  - [ ] Smooth
  - [x] Subdivision
- [x] **P4: Tessellation**
  - [x] Sphere
    - [x] Wireframe
    - [x] Flat/Smooth
  - [x] Cylinder
    - [x] Wireframe
    - [x] Flat/Smooth
  - [x] Cone
    - [x] Wireframe
    - [x] Flat/Smooth
- [x] **P5: Torus**
  - [x] Wireframe
  - [x] Flat
  - [ ] Smooth
  - [x] Subdivision
- [x] **P6: Super-quqdrics And Dodecahedron**
  - [x] Super-quqdrics
    - [x] Wireframe
    - [x] Flat/Smooth
    - [ ] Dynamically Load Parameters
  - [x] Dodecahedron
    - [x] Wireframe
    - [x] Flat
    - [ ] Smooth
    - [x] Subdivision
- [ ] **P7: Flight Simulation**
  - [x] City Scene Assembly (Has 8-12 urban structures)
  - [x] Display
    - [x] Wireframe
    - [x] Flat
    - [ ] Smooth
  - [ ] Loops
    - [ ] Horizontal Loop
    - [ ] Vertical Loop
- [ ] **P8: Bonus**
  - [ ] Normal Display Mode
  - [ ] Other (Please Specify)

## Usage

- If you have implemented extra functionalities not mentioned in the manual, you may specify them here.
- If your program failed to obey the required mouse/keyboard gestures, you may also specify your own setting here. In this case, penalties may apply.
