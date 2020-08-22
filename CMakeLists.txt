cmake_minimum_required(VERSION 3.16)
project(PA3)

link_libraries(glfw GLEW GL)

include_directories(".")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

add_executable(PA3 mainProject.cpp LightingSystem/LightSource.cpp Models/Model.cpp Models/Skybox.cpp Terrain/Terrain.cpp Terrain/PerlinNoise.cpp Terrain/City.cpp)