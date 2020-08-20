#pragma once
#include "Terrain.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

Terrain::Terrain() : Terrain(glm::vec3(10), 16){}
Terrain::Terrain(glm::vec3 size, int res) {
    pn = PerlinNoise();
    mapSize = size;
    resolution = res;
    heightMap = generateHeightMap();

    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    std::vector<glm::vec3> colors = std::vector<glm::vec3>();
    std::vector<glm::vec3> normals = std::vector<glm::vec3>();
    std::vector<glm::vec2> UVs = std::vector<glm::vec2>();

    glm::vec3 scale = mapSize / glm::vec3(resolution - 1, 1.0f, resolution - 1);

    float centerOffset = (float)(resolution - 1) / 2.0f;

    for (float i = 0; i < resolution - 1; i += 1.0f) {
        for (float j = 0; j < resolution - 1; j += 1.0f) {
            vertices.push_back(glm::vec3(i - centerOffset, getNoiseAt(i, j), j - centerOffset) * (scale));
            vertices.push_back(glm::vec3(i - centerOffset, getNoiseAt(i, j+1.0f), j+1.0f - centerOffset) * (scale));
            vertices.push_back(glm::vec3(i+1.0f - centerOffset, getNoiseAt(i+1.0f, j), j - centerOffset) * (scale));

            vertices.push_back(glm::vec3(i+1.0f - centerOffset, getNoiseAt(i+1.0f, j), j - centerOffset) * (scale));
            vertices.push_back(glm::vec3(i - centerOffset, getNoiseAt(i, j+1.0f), j+1.0f - centerOffset) * (scale));
            vertices.push_back(glm::vec3(i+1.0f - centerOffset, getNoiseAt(i+1.0f, j+1.0f), j+1.0f - centerOffset) * (scale));
        }
    }

    for (float i = 0; i < resolution - 1; i += 1.0f) {
        for (float j = 0; j < resolution - 1; j += 1.0f) {
            colors.push_back(glm::vec3(1));
            colors.push_back(glm::vec3(1));
            colors.push_back(glm::vec3(1));

            colors.push_back(glm::vec3(1));
            colors.push_back(glm::vec3(1));
            colors.push_back(glm::vec3(1));
        }
    }

    for (float i = 0; i < resolution - 1; i += 1.0f) {
        for (float j = 0; j < resolution - 1; j += 1.0f) {
            normals.push_back(getNormalAt(i, j));
            normals.push_back(getNormalAt(i, j+1));
            normals.push_back(getNormalAt(i+1, j));

            normals.push_back(getNormalAt(i+1, j));
            normals.push_back(getNormalAt(i, j+1));
            normals.push_back(getNormalAt(i+1, j+1));
        }
    }

    for (float i = 0; i < resolution - 1; i += 1.0f) {
        for (float j = 0; j < resolution - 1; j += 1.0f) {
            UVs.push_back(glm::vec2(i / resolution, j / resolution));
            UVs.push_back(glm::vec2(i / resolution, (j+1) / resolution));
            UVs.push_back(glm::vec2((i+1) / resolution, j / resolution));

            UVs.push_back(glm::vec2((i+1) / resolution, j / resolution));
            UVs.push_back(glm::vec2(i / resolution, (j+1) / resolution));
            UVs.push_back(glm::vec2((i+1) / resolution, (j+1) / resolution));
        }
    }

    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO); //Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

    std::cout << terrainVAO << std::endl;

    //Vertex VBO setup
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    //Colors VBO setup
    GLuint colors_VBO;
    glGenBuffers(1, &colors_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1);

    //Normals VBO setup
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(2);

    //UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs, as we are using multiple VAOs)

    numberOfVertices = vertices.size();

    std::vector<glm::vec3>().swap(vertices);
    std::vector<glm::vec3>().swap(colors);
    std::vector<glm::vec3>().swap(normals);
    std::vector<glm::vec2>().swap(UVs);
}

GLuint Terrain::getVAO() {
    return terrainVAO;
}

int Terrain::getNumberVertices() {
    return numberOfVertices;
}

float Terrain::getNoiseAt(float x, float y, float z) {
    float dx = ((float)x - ((float)(resolution-1) / (float)2)) / (float)(resolution-1);
    float dy = ((float)y - ((float)(resolution-1) / (float)2)) / (float)(resolution-1);

    float noise = 2.0f * pn.noise(dx, dy, 0.1) // Low detail
                  + 2 * pn.noise(dx, dy, 0.2) * pn.noise(dx * 4.0f, dy * 4.0f, 0.1)
                  + (1.0f / 16.0f) * pn.noise(dx * 16.0f, dy * 16.0f, z);

    return noise;
}

std::vector<float> Terrain::generateHeightMap() {
    std::vector<float> map = std::vector<float>();

    for (unsigned int i = 0; i < resolution; i++) {
        for (unsigned int j = 0; j < resolution; j++) {
            map.push_back(getNoiseAt((float)i, (float)j) * mapSize.y);
        }
    }

    return map;
}



float Terrain::getHeightAt(float x, float z) {
    return getNoiseAt(x, z) * mapSize.y;
//    TODO: fix this method
//    if (x < 0 || x >= resolution || z < 0 || z >= resolution) {
//        return getNoiseAt(x, z);
//    } else {
//        float height = heightMap[(int)x * resolution + (int)z];
//
//        return height;
//    }
}

glm::vec3 Terrain::getNormalAt(float x, float z) {
    if (x < 0 || x >= resolution || z < 0 || z >= resolution) {
        return glm::vec3(0);
    } else {
        float offX = (mapSize.x / mapSize.z) / (resolution - 1);
        float offZ = (mapSize.z / mapSize.x) / (resolution - 1);

        float hL = getHeightAt(x - offX, z);
        float hR = getHeightAt(x + offX, z);
        float hD = getHeightAt(x, z - offZ);
        float hU = getHeightAt(x, z + offZ);

        glm::vec3 normal = glm::vec3(hL-hR, 2.0, hD-hU);
        normal = glm::normalize(normal);

        return normal;
    }
}
