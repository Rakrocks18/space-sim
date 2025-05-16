# Space Simulation

This code tries to emulate the famous space-time fabric proposed by Einstein.

## Requirements
1. Clang/Clang++
2. Freeglut/Glut (OpenGL)
3. glm (for mathematics)

## Run the code
#### Windows:
```bash
.\compile_run.bat
```
#### MacOS & Linux
```bash
clang++ src/main.cpp -o bin/application -I/mingw64/include -L/mingw64/lib -lglfw3 -lglew32 -lopengl32 -lgdi32 -lpthread
.\bin\application
```