#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>

class Model {
	int vao;
	int numberOfVertices;
	std::vector<Model*> children;
	glm::mat4 translation, rotation, scaling;

public:
	Model(int vao, int numberOfVertices, std::vector<Model*> children);
	Model(int vao, int numberOfVertices, std::vector<Model*> children, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling);
	void const draw(glm::mat4 parentTRS, int renderingMode, GLuint worldMatrixLocation);
};