# ObjLoader
Load OBJ files along with accompanying DDS textures, and look at it from any angle 
using FPS style controls with mouse and keyboard

[![Project Demo](https://img.youtube.com/vi/CxJfKVWL0Do/0.jpg)](https://www.youtube.com/watch?v=CxJfKVWL0Do)

# Getting Dependencies
```
# To install dependencies in Ubuntu
sudo apt install build-essential
sudo apt install libgl1-mesa-dev
sudo apt install libglu1-mesa-dev
sudo apt install freeglut3-dev 
sudo apt install libfreeimage-dev
sudo apt install libglfw3-dev
sudo apt install libglm-dev

# To install dependencies in Fedora
sudo dnf install make gcc-c++
sudo dnf install mesa-libGL mesa-libGLU mesa-libGLU-devel mesa-libGL-devel 
sudo dnf install glm-devel freeimage-devel freeglut-devel glfw-devel 
```

# To compile
```
# To Compile the program
g++ definitions/* loaders/* glad.c main.cpp -o app -lGL -lGLU -lX11 -ldl -lglfw -lfreeimage
```

# To run with IIITM model
```
#To execute
./app models/IIITM_LOGO.obj models/IIITM_UV_MAP.dds
```