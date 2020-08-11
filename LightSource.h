#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>

class LightSource {
	glm::vec3 lightPosition = glm::vec3(0.0f);
	glm::mat4 projection, view;
	GLuint shadowMap;
	GLuint shadowMapBuffer;
	int textureIndex;
	float shadowMapSize;

public:
	LightSource(glm::vec3 lightPosition, glm::mat4 projection, glm::mat4 view, float shadowMapSize, int textureIndex);
	GLsizeiptr const setDataForDrawing(GLsizeiptr currentLocation);
	void const setDataForShadowMap(GLuint uboDepthVPBlock);
	void const bindShadowMapTexture();

private:
	glm::mat4 const getViewProjectionMatrix() { return projection * view; }
	void createShadowMapBuffer();
};