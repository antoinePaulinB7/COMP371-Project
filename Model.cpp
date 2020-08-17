// Recursive hierarchical modelling class
// Author: Nicole Parmentier

#include "Model.h"
#include <iostream>

using namespace std;
using namespace glm;

Material DEFAULT_MATERIAL = {};

Model::Model(int vao, int numberOfVertices, unsigned int uboWorldMatrixBlock, vector<Model*> children) : Model(vao, numberOfVertices, uboWorldMatrixBlock, children, DEFAULT_MATERIAL) {}
Model::Model(int vao, int numberOfVertices, unsigned int uboWorldMatrixBlock, vector<Model*> children, Material material) : Model(vao, numberOfVertices, uboWorldMatrixBlock, children, mat4(1.0f), mat4(1.0f), mat4(1.0f), material) {}
Model::Model(int vao, int numberOfVertices, unsigned int uboWorldMatrixBlock, vector<Model*> children, mat4 translation, mat4 rotation, mat4 scaling) : Model(vao, numberOfVertices, uboWorldMatrixBlock, children, translation, rotation, scaling, DEFAULT_MATERIAL) {}
Model::Model(int vao, int numberOfVertices, unsigned int uboWorldMatrixBlock, vector<Model*> children, mat4 translation, mat4 rotation, mat4 scaling, Material material) {
	this->children = children;
	this->vao = vao;
	this->numberOfVertices = numberOfVertices;
	this->translation = translation;
	this->rotation = rotation;
	this->scaling = scaling;
    this->material = material;
    this->uboWorldMatrixBlock = uboWorldMatrixBlock;
}

void const Model::draw(mat4 parentTRS, int renderingMode, GLuint lightCoeffsLocation, GLuint lightColorLocation) {
  if(material.texture != 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.texture);
  }
  glBindVertexArray(vao);
	mat4 currentTRS = parentTRS * translation * rotation * scaling;

	//A model which just holds and transforms children has no vertices and does not need to be drawn, only calculated
	if (numberOfVertices > 0) {
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

		glDrawArrays(renderingMode, 0, numberOfVertices);
	}

	for (vector<Model*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->draw(currentTRS, renderingMode, lightCoeffsLocation, lightColorLocation);
	}

  if(material.texture != 0) {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

Material getDefaultMaterial() {
  return DEFAULT_MATERIAL;
}
