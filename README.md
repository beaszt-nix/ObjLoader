# ObjLoader
Load OBJ files along with accompanying DDS textures, and look at it from any angle 
using FPS style controls with mouse and keyboard

# To compile
```
g++ definitions/* loaders/* glad.c main.cpp -lGL -lGLU -lm -lX11 -ldl -lglfw
```

# To run with IIITM model
```
./a.out models/IIITM_LOGO.obj models/IIITM_UV_MAP.dds
```

# How it look
[Video](https://www.youtube.com/watch?v=AVmbay6Znhg&feature=youtu.be)
