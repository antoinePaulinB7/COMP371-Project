#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>

struct Material {
  Material() : texture(1), lightCoefficients(glm::vec4(1.0f)), lightColor(glm::vec3(1.0f)) {}
  GLuint texture;
  glm::vec4 lightCoefficients;
  glm::vec3 lightColor;
};

class Model {
	int vao;
	int numberOfVertices;
	std::vector<Model*> children;
	glm::mat4 translation, rotation, scaling;
  Material material;

public:
	Model(int vao, int numberOfVertices, std::vector<Model*> children);
  Model(int vao, int numberOfVertices, std::vector<Model*> children, Material material);
	Model(int vao, int numberOfVertices, std::vector<Model*> children, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling);
	Model(int vao, int numberOfVertices, std::vector<Model*> children, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling, Material material);
	void const draw(glm::mat4 parentTRS, int renderingMode, GLuint worldMatrixLocation, GLuint lightCoeffsLocation, GLuint lightColorLocation);
  Material getDefaultMaterial();
};