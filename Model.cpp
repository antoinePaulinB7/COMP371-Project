// Recursive hierarchical modelling class
// Author: Nicole Parmentier


#include "Model.h"

using namespace std;
using namespace glm;


Model::Model(int vao, int numberOfVertices, vector<Model*> children) : Model(vao, numberOfVertices, children, mat4(1.0f), mat4(1.0f), mat4(1.0f)) {}


Model::Model(int vao, int numberOfVertices, vector<Model*> children, mat4 translation, mat4 rotation, mat4 scaling){
	this->children = children;
	this->vao = vao;
	this->numberOfVertices = numberOfVertices;
	this->translation = translation;
	this->rotation = rotation;
	this->scaling = scaling;
}

//draw function which takes a shader class / program in order to write to its uniforms
//draw is recursive, draw self(multiply recieved TRS with own) then draw children(passing along its TRS)
void const Model::draw(mat4 parentTRS, int renderingMode, GLuint worldMatrixLocation) {
	glBindVertexArray(vao);
	mat4 currentTRS = parentTRS * translation * rotation * scaling;

	//A model which just holds and transforms children has no vertices and does not need to be drawn, only calculated
	if (numberOfVertices > 0) {
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &currentTRS[0][0]);
		glDrawArrays(renderingMode, 0, numberOfVertices);
	}

	//if (children != null) {
	for (vector<Model*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->draw(currentTRS, renderingMode, worldMatrixLocation);
	}
	//}
}
