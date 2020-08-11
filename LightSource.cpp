// Light source class
// Author: Nicole Parmentier

#include "LightSource.h"
#include <iostream>

using namespace std;
using namespace glm;

LightSource::LightSource(vec3 lightPosition, mat4 projection, mat4 view) {
	this->lightPosition = lightPosition;
	this->projection = projection;
	this->view = view;
}

GLsizeiptr const LightSource::setDataForDrawing(GLsizeiptr currentLocation) {
	mat4 depthVP = this->getViewProjectionMatrix();
	glBufferSubData(GL_UNIFORM_BUFFER, currentLocation, sizeof(glm::mat4), &depthVP[0][0]);
	currentLocation += sizeof(glm::mat4);
	glBufferSubData(GL_UNIFORM_BUFFER, currentLocation, sizeof(glm::vec3), &this->lightPosition);
	currentLocation += sizeof(glm::vec3);
	return currentLocation;
}

void const LightSource::setDataForShadowMap(GLuint uboDepthVPBlock) {
	mat4 depthVP = this->getViewProjectionMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboDepthVPBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &depthVP[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

