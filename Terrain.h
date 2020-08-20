#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "PerlinNoise.h"

class Terrain {
    PerlinNoise pn;
    glm::vec3 mapSize;
    int resolution;
    GLuint terrainVAO;
	std::vector<glm::vec3> vertexPositions;

public:
    Terrain();
    Terrain(glm::vec3 size, int resolution);

    float getHeightAt(float x, float z);
    glm::vec3 getNormalAt(float x, float z);
    GLuint getVAO();
	std::vector<glm::vec3> getVertices();
private:
    std::vector<float> heightMap;
    std::vector<float> generateHeightMap();
    float getNoiseAt(float x, float y, float z = 0.1f);
};

#endif
