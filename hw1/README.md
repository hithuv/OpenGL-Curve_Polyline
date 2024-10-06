# HW1

Srihith Bharadwaj Burra

116728900

srihithbharadw.burra@stonybrook.edu
srburra@cs.stonybrook.edu

## Overview

- Implemented a sample modern OpenGL program with GLFW as the windowing toolkit. 
- Implemented a naive Bresenham line drawing routine without edge-case handling. 
- Implemented Bresenham for all slopes and vertical lines.
- Implemented Polyline drawing, with completion for polygon, and scanline + error reporting (intersecting red lines)
- Implemented Ellipse (left-click) and Circle (hold shift while left-click) drawing
- Implemented drawing of polynomials of form: y(x) = a3x3 + a2x2 + a1x + a0 (Using modified bresenham)
## Dependencies

- OpenGL (Required for Both Versions):
```bash
sudo add-apt-repository ppa:kisak/kisak-mesa
sudo apt update
sudo apt-get dist-upgrade
sudo reboot
```

## Compile & Run

- C/C++ Version (Run inside `cpp/`): 
```
./build_script.sh
```

## Features Implemented

Check all features implemented with "x" in "[ ]"s. 
Features or parts left unchecked here won't be graded! 

- [x] 1. Line Segment (Fully Implemented in This Template)
  - [x] 0 <= m <= 1
- [x] 2. Line Segment
  - [x] Slope m < -1
  - [x] -1 <= m < 0
  - [x] 1 < m
  - [x] Vertical
- [x] 3. Poly-line & Polygon
  - [x] Poly-line
  - [x] Polygon
- [x] 4. Circle & Ellipse
  - [x] Circle
  - [x] Ellipse
- [x] 5. Polynomial Curve (BONUS PART-1)
  - [x] Line
  - [x] Quadratic Curve
  - [x] Cubic Curve
- [x] 6. Scan-conversion (BONUS PART-2)
  - [x] Triangle
  - [x] Convex Polygon
  - [x] Concave Polygon
  - [x] Self-intersection detection & report

## Usage

Implemented all the requirements as per manual. Project has been coded in a ARM based OS, so if some issue arises while running, please let me know. Sorry if any incovenience caused.


## Appendix

Please include any other stuff you would like to mention in this section.
E.g., your suggestion on possible combinations of cubic curve parameters in this programming part.

0.0001 -0.12 +44 -4800
-0.0001 +0.12 -44 +4800
0 0.005 -2.00 250.0
0 -0.005 2.00 350.0
0.0 0.0 0 25
0.0 0.0 1.1 -0.1
