# HW2

Srihith Bharadwaj Burra

116728900

srburra@cs.stonybrook.edu

## Overview

- Implemented a sample modern OpenGL program with GLFW as the windowing toolkit. 
- Implemented some basic geometries, including lines, triangles, and circles. Circles are implemented with tessellation shaders. 

## Notes

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


```

## Compile & Run

- C/C++ Version (Run inside `cpp/`): 
```bash
mkdir build
cd build
cmake -DMAKE_BUILD_TYPE=Release ..
make 
cd ..
./build/hw2
```

```

## Features Implemented

Check all features implemented with "x" in "[ ]"s. 
Features or parts left unchecked here won't be graded! 

- [x] 1. One Segment of Cubic Bezier Spline
- [x] 2. Piecewise C(2) Cubic Bezier Spline
  - [x] Base Spline Creation And Display
  - [x] Control Node Dragging
  - [x] Control Node Insertion CANT INSERT JUST A SINGLE NODE
  - [x] Control Node Deletion
  - [x] Save to File
  - [x] Load from File
- [x] 3. Catmull-Rom Spline
  - [x] Base Spline Creation And Display
  - [x] Interpolation Point Dragging
  - [x] Interpolation Point Insertion
  - [x] Interpolation Point Deletion
  - [x] Save to File
  - [x] Load from File
- [x] 4. Extension to 3D (BONUS)
  - [x] Tracking Ball
  - [x] One Segment of Cubic Bezier Spline
  - [x] Piecewise C(2) Bezier Spline
  - [x] Catmull-Rom Spline

## Usage

If Insert or delete keys do not delete a point, please use key 'I' to insert and key 'BACKSPACE' to delete.
Selecting a point turns into white, can insert, or delete now.
Direct click and drag works, and indicated by point in white.
In 3D mode, should select either key 1(bezier) or key 3(catmullRom)
W and S : camera move in z axis
A and D : camera move x axis
Q and E : camera move y axis
R camera rotates the points CCW So can add new points


