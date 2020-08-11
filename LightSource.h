#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>

class LightSource {
	glm::vec3 lightPosition = glm::vec3(0.0f);
	glm::mat4 projection, view;

public:
	LightSource(glm::vec3 lightPosition, glm::mat4 projection, glm::mat4 view);
	GLsizeiptr const setDataForDrawing(GLsizeiptr currentLocation);
	void const setDataForShadowMap(GLuint uboDepthVPBlock);

private:
	glm::mat4 const getViewProjectionMatrix() { return projection * view; }
};