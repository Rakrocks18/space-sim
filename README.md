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
clang++ src/main.cpp -o bin/application.exe -I/mingw64/include -L/mingw64/lib -lopengl32 -lglu32 -lfreeglut -lgdi32
.\bin\application.exe
```