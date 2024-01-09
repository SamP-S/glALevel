g++ src/OpenGL_0.1.cpp -o bin/v0.1 -Iexternal/GLFW/include -Lexternal/GLFW/lib -lglfw
g++ src/OpenGL_0.2.cpp -o bin/v0.2 -Iexternal/GLFW/include -Iexternal/GLEW/include -Lexternal/GLFW/lib -Lexternal/GLEW/lib -lGLEW -lGL -lglfw 
g++ src/OpenGL_0.3.cpp -o bin/v0.3 -Iexternal/GLFW/include -Iexternal/GLEW/include -Lexternal/GLFW/lib -Lexternal/GLEW/lib -lGLEW -lGL -lglfw 
