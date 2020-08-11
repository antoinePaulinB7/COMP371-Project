// Light source class
// Author: Nicole Parmentier

#include "LightSource.h"
#include <iostream>

using namespace std;
using namespace glm;

LightSource::LightSource(vec3 lightPosition, mat4 projection, mat4 view, float shadowMapSize, int textureIndex) {
	this->lightPosition = lightPosition;
	this->projection = projection;
	this->view = view;
	this->shadowMapSize = shadowMapSize;
	this->textureIndex = textureIndex;
	createShadowMapBuffer();
}

GLsizeiptr const LightSource::setDataForDrawing(GLsizeiptr currentLocation) {
	mat4 depthVP = getViewProjectionMatrix();
	glBufferSubData(GL_UNIFORM_BUFFER, currentLocation, 64, &depthVP[0][0]);
	currentLocation += 64;
	glBufferSubData(GL_UNIFORM_BUFFER, currentLocation, 16, &lightPosition);
	currentLocation += 16;
	return currentLocation;
}

void const LightSource::setDataForShadowMap(GLuint uboDepthVPBlock) {
	mat4 depthVP = getViewProjectionMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboDepthVPBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &depthVP[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//bind and clear the shadow buffer, set viewport to shadowMap dimensions
	glViewport(0, 0, shadowMapSize, shadowMapSize);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void const LightSource::bindShadowMapTexture() {
	glActiveTexture(textureIndex);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}

//The buffer is the memory that backs up the shadowMap texture, like how the VBO is the memory that backs up the VAO
// referenced this tutorial which was suggested in the labs https://learnopengl.com/Getting-started/Textures
// which lead to this tutorial https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
// referenced the opengl docs for framebuffer info https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glFramebufferTexture2D.xml
void LightSource::createShadowMapBuffer()
{
	//create a frame buffer to hold the shadow map data
	GLuint frameBufferObject;
	glGenFramebuffers(1, &frameBufferObject);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);	//Set up an empty border, instead of having the default repeating texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//which lead to duplicate shadows out of place!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cerr << "error generating shadow map buffer" << endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	shadowMapBuffer = frameBufferObject;
}

