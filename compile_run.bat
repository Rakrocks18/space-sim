@echo off
clang++ src/main.cpp -o bin/application.exe -I/mingw64/include -L/mingw64/lib -lglfw3 -lglew32 -lopengl32 -lgdi32 -lpthread
.\bin\application.exe