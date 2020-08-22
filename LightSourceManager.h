#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>

using namespace glm;

const int totalNumLights = 2;
int currentLightIndex = 0;

GLuint uboDepthVPBlockCopy;

LightSource* lights[totalNumLights];
vec3 lightPositions[totalNumLights] = { vec3(0.0f, 5.0f, 30.0f)};	//1
//										vec3(-45, 30.0f, -45)};

vec3 lightLookAtPositions[totalNumLights] = { vec3(0.0f, 0.0f, 0.0f)};	//1
//										vec3(-45, 0.0f, -45)};

void setUBODepthVPBlockInLightSourceManager(GLuint value) {
	uboDepthVPBlockCopy = value;
}

void createLightSources()
{
	const float bigShadowMapSize = 2048, smallShadowMapSize = 1024;
	mat4 lightProjectionMatrix = perspective(180.0f, 1.0f, 0.01f, 2048.0f);
	currentLightIndex = 0;
	while (currentLightIndex < totalNumLights) {
		lights[currentLightIndex] = new LightSource(lightPositions[currentLightIndex], lightLookAtPositions[currentLightIndex],
			(currentLightIndex == 0 ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f)), lightProjectionMatrix, 
			bigShadowMapSize, GL_TEXTURE1 + currentLightIndex);

		currentLightIndex++;
	}
	currentLightIndex = 0;
}

bool makeShadowMapForNextLight() {
	if (currentLightIndex < totalNumLights) {
		lights[currentLightIndex]->setDataForShadowMap(uboDepthVPBlockCopy);
		currentLightIndex++;
		return true;
	}
	else {
		currentLightIndex = 0;
		return false;
	}
}

void bindShadowMaps(int lightShader)
{
	GLint shadowMapLoc = glGetUniformLocation(lightShader, "shadowMap");
	GLint shadowMapLoc2 = glGetUniformLocation(lightShader, "shadowMap2");
	glUniform1i(shadowMapLoc, 1);
	glUniform1i(shadowMapLoc2, 2);
	currentLightIndex = 0;
	while (currentLightIndex < totalNumLights) {
		lights[currentLightIndex]->bindShadowMapTexture();
		currentLightIndex++;
	}
	currentLightIndex = 0;
}

void setDataForDrawingLights() {
	GLsizeiptr currentLocation = 0;
	currentLightIndex = 0;
	while (currentLightIndex < totalNumLights) {
		currentLocation = lights[currentLightIndex]->setDataForDrawing(currentLocation);
		currentLightIndex++;
	}
	currentLightIndex = 0;
}

void drawLightSources(GLuint worldMatrixLocation, int sphereVertices) {
	currentLightIndex = 0;
	while (currentLightIndex < totalNumLights) {

		mat4 lightPos = translate(mat4(1.0f), lights[currentLightIndex]->getPosition());
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lightPos[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, sphereVertices);
		currentLightIndex++;
	}
	currentLightIndex = 0;
}

void updateFlashlight(vec3 position, vec3 lookAtPosition) {
	lights[0]->setPosition(position);
	lights[0]->setLookAtPosition(lookAtPosition);
}