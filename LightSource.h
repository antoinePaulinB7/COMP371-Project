#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>

class LightSource {
	glm::vec3 lightPosition = glm::vec3(0.0f);
	glm::vec3 lookAtPos;
	glm::vec3 upVector;
	glm::mat4 projection;
	GLuint shadowMap;
	GLuint shadowMapBuffer;
	int textureIndex;
	float shadowMapSize;

public:
	LightSource(glm::vec3 lightPosition, glm::vec3 lookAtPos, glm::vec3 upVector, glm::mat4 projection, float shadowMapSize, int textureIndex);
	GLsizeiptr const setDataForDrawing(GLsizeiptr currentLocation);
	void const setDataForShadowMap(GLuint uboDepthVPBlock);
	void const bindShadowMapTexture();
	void setPosition(glm::vec3 newPos) { lightPosition = newPos; }
	void setLookAtPosition(glm::vec3 newPos) { lookAtPos = newPos; }
	glm::vec3 getPosition() { return lightPosition; }

private:
	glm::mat4 const getViewMatrix() { return glm::lookAt(lightPosition,  // eye
														lookAtPos,  // center
														upVector); // up; 
									}
	glm::mat4 const getViewProjectionMatrix() { return projection * getViewMatrix(); }
	void createShadowMapBuffer();
};