// Recursive hierarchical modelling class
// Author: Nicole Parmentier

#include "Skybox.h"
#include <iostream>

using namespace std;
using namespace glm;

Skybox::Skybox(int vao, int numberOfVertices, unsigned int uboWorldMatrixBlock, std::vector<Model*> children, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling,
	Material material) : Model(vao, numberOfVertices, uboWorldMatrixBlock, children, translation, rotation, scaling, material) {};


void const Skybox::draw(mat4 parentTRS, int renderingMode, GLuint lightCoeffsLocation, GLuint lightColorLocation) {
	if (material.texture != 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material.texture);
	}
	glBindVertexArray(vao);
	mat4 currentTRS = parentTRS * translation * rotation * scaling;
	glBindBuffer(GL_UNIFORM_BUFFER, uboWorldMatrixBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &currentTRS[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glUniform4f(
		lightCoeffsLocation,
		material.lightCoefficients[0], // Ambient
		material.lightCoefficients[1], // Diffuse
		material.lightCoefficients[2], // Specular
		material.lightCoefficients[3]  // Shininess
	);

	glUniform3f(
		lightColorLocation,
		material.lightColor[0], //R
		material.lightColor[1], //G
		material.lightColor[2]  //B
	);


	glDisable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);

	glDrawArrays(renderingMode, 0, numberOfVertices);
	
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	if (material.texture != 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

