# Fibonacci-Visualizer
A Qt/C++ application that visualizes the recursive Fibonacci call tree.  
(Short one-line description of purpose)

## Features
- Interactive visualization of recursion tree
- Node highlighting and timing
- Configurable n (max Fibonacci index)
- Export screenshots / gifs (if available)

## Screenshot
![screenshot](./screenshots/preview.png)

## Requirements
- Qt 6.x (or Qt 5.x) — specify exact version
- C++17
- CMake 3.16+ (or qmake if used)

## Build (CMake)
```bash
mkdir build
cd build
cmake ..            # -DCMAKE_PREFIX_PATH=/path/to/Qt if needed
cmake --build .
