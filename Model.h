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
protected:
	int vao;
	int numberOfVertices;
	unsigned int uboWorldMatrixBlock;
	std::vector<Model*> children;
	glm::mat4 translation, rotation, scaling;
	glm::mat4 currentTRS;
	Material material;
	std::vector<glm::vec3> vertexPositions;

public:
	Model(int vao, std::vector<glm::vec3> vertexPositions, unsigned int uboWorldMatrixBlock, std::vector<Model*> children, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling);
	Model(int vao, std::vector<glm::vec3> vertexPositions, unsigned int uboWorldMatrixBlock, std::vector<Model*> children, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling, Material material);
	virtual void const draw(glm::mat4 parentTRS, int renderingMode, GLuint lightCoeffsLocation, GLuint lightColorLocation);
	Material getDefaultMaterial();
	float IntersectsRay(glm::vec3 source, glm::vec3 direction);
};