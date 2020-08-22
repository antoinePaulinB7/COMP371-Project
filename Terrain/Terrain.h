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

public:
    Terrain();
    Terrain(glm::vec3 size, glm::vec3 offset, int resolution);

    float getHeightAt(float x, float z);
    glm::vec3 getNormalAt(float x, float z);
    GLuint getVAO();
	std::vector<glm::vec3> getVertices();
private:
    glm::vec3 mapSize, offset;
    int resolution;
    GLuint terrainVAO;
    std::vector<glm::vec3> vertexPositions;
    std::vector<float> heightMap;
    std::vector<float> generateHeightMap();
    float getNoiseAt(float x, float y, float z = 0.1f);
};

#endif
