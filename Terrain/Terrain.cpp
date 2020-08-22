#pragma once
#include "Terrain.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

Terrain::Terrain() : Terrain(glm::vec3(10), glm::vec3(0), 16){}
Terrain::Terrain(glm::vec3 size, glm::vec3 offset, int res) {
    pn = PerlinNoise();
    this->mapSize = size;
    this->offset = offset;
    this->resolution = res;
    this->heightMap = generateHeightMap();

    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    std::vector<glm::vec3> colors = std::vector<glm::vec3>();
    std::vector<glm::vec3> normals = std::vector<glm::vec3>();
    std::vector<glm::vec2> UVs = std::vector<glm::vec2>();

    glm::vec3 scale = this->mapSize / glm::vec3(this->resolution - 1, 1.0f, this->resolution - 1);

    float centerOffset = (float)(this->resolution - 1) / 2.0f;

    for (float i = 0; i < this->resolution - 1; i += 1.0f) {
        for (float j = 0; j < this->resolution - 1; j += 1.0f) {
            vertices.push_back(glm::vec3(i - centerOffset, getNoiseAt(i, j), j - centerOffset) * (scale));
            vertices.push_back(glm::vec3(i - centerOffset, getNoiseAt(i, j+1.0f), j+1.0f - centerOffset) * (scale));
            vertices.push_back(glm::vec3(i+1.0f - centerOffset, getNoiseAt(i+1.0f, j), j - centerOffset) * (scale));

            vertices.push_back(glm::vec3(i+1.0f - centerOffset, getNoiseAt(i+1.0f, j), j - centerOffset) * (scale));
            vertices.push_back(glm::vec3(i - centerOffset, getNoiseAt(i, j+1.0f), j+1.0f - centerOffset) * (scale));
            vertices.push_back(glm::vec3(i+1.0f - centerOffset, getNoiseAt(i+1.0f, j+1.0f), j+1.0f - centerOffset) * (scale));
        }
    }

    for (float i = 0; i < this->resolution - 1; i += 1.0f) {
        for (float j = 0; j < this->resolution - 1; j += 1.0f) {
            colors.push_back(glm::vec3(1));
            colors.push_back(glm::vec3(1));
            colors.push_back(glm::vec3(1));

            colors.push_back(glm::vec3(1));
            colors.push_back(glm::vec3(1));
            colors.push_back(glm::vec3(1));
        }
    }

    for (float i = 0; i < this->resolution - 1; i += 1.0f) {
        for (float j = 0; j < this->resolution - 1; j += 1.0f) {
            normals.push_back(getNormalAt(i, j));
            normals.push_back(getNormalAt(i, j+1));
            normals.push_back(getNormalAt(i+1, j));

            normals.push_back(getNormalAt(i+1, j));
            normals.push_back(getNormalAt(i, j+1));
            normals.push_back(getNormalAt(i+1, j+1));
        }
    }

    for (float i = 0; i < this->resolution - 1; i += 1.0f) {
        for (float j = 0; j < this->resolution - 1; j += 1.0f) {
            UVs.push_back(glm::vec2(i / this->resolution, j / this->resolution));
            UVs.push_back(glm::vec2(i / this->resolution, (j+1) / this->resolution));
            UVs.push_back(glm::vec2((i+1) / this->resolution, j / this->resolution));

            UVs.push_back(glm::vec2((i+1) / this->resolution, j / this->resolution));
            UVs.push_back(glm::vec2(i / this->resolution, (j+1) / this->resolution));
            UVs.push_back(glm::vec2((i+1) / this->resolution, (j+1) / this->resolution));
        }
    }

    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO); //Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

    //std::cout << terrainVAO << std::endl;

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

	vertexPositions = vertices;

    std::vector<glm::vec3>().swap(vertices);
    std::vector<glm::vec3>().swap(colors);
    std::vector<glm::vec3>().swap(normals);
    std::vector<glm::vec2>().swap(UVs);
}

GLuint Terrain::getVAO() {
    return terrainVAO;
}

std::vector<glm::vec3> Terrain::getVertices() {
    return vertexPositions;
}

float Terrain::getNoiseAt(float x, float y, float z) {
    float dx = ((float)x + this->offset.x - ((float)(this->resolution-1) / (float)2)) / (float)(this->resolution-1);
    float dy = ((float)y + this->offset.z - ((float)(this->resolution-1) / (float)2)) / (float)(this->resolution-1);

    float noise = 2.0f * pn.noise(dx, dy, 0.1) // Low detail
                  + 2 * pn.noise(dx, dy, 0.2) * pn.noise(dx * 4.0f, dy * 4.0f, 0.1)
                  + (1.0f / 16.0f) * pn.noise(dx * 16.0f, dy * 16.0f, z);

    return noise;
}

std::vector<float> Terrain::generateHeightMap() {
    std::vector<float> map = std::vector<float>();

    for (unsigned int i = 0; i < this->resolution; i++) {
        for (unsigned int j = 0; j < this->resolution; j++) {
            map.push_back(getNoiseAt((float)i, (float)j) * this->mapSize.y);
        }
    }

    return map;
}



float Terrain::getHeightAt(float x, float z) {
    float centerOffset = (float)(this->resolution - 1) / 2.0f;

    glm::vec3 scale = this->mapSize / glm::vec3(this->resolution - 1, 1.0f, this->resolution - 1);

    return getNoiseAt(x / scale.x + centerOffset, z / scale.z + centerOffset) * this->mapSize.y;
}

glm::vec3 Terrain::getNormalAt(float x, float z) {
    if (x < 0 || x >= this->resolution || z < 0 || z >= this->resolution) {
        return glm::vec3(0);
    } else {
        float offX = (this->mapSize.x / this->mapSize.z) / (this->resolution - 1);
        float offZ = (this->mapSize.z / this->mapSize.x) / (this->resolution - 1);

        float hL = getNoiseAt(x - offX, z) * this->mapSize.y;
        float hR = getNoiseAt(x + offX, z) * this->mapSize.y;
        float hD = getNoiseAt(x, z - offZ) * this->mapSize.y;
        float hU = getNoiseAt(x, z + offZ) * this->mapSize.y;

        glm::vec3 normal = glm::vec3(hL-hR, 2.0, hD-hU);
        normal = glm::normalize(normal);

        return normal;
    }
}
