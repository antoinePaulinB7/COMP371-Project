// Recursive hierarchical modelling class
// Author: Nicole Parmentier

#include "Model.h"
#include <iostream>

using namespace std;
using namespace glm;

Material DEFAULT_MATERIAL = {};

Model::Model(int vao, vector<vec3> vertexPositions, unsigned int uboWorldMatrixBlock, vector<Model*> children, mat4 translation, mat4 rotation, mat4 scaling) : Model(vao, vertexPositions, uboWorldMatrixBlock, children, translation, rotation, scaling, DEFAULT_MATERIAL) {}
Model::Model(int vao, vector<vec3> vertexPositions, unsigned int uboWorldMatrixBlock, vector<Model*> children, mat4 translation, mat4 rotation, mat4 scaling, Material material) {
	this->children = children;
	this->vao = vao;
	this->numberOfVertices = vertexPositions.size();
	this->vertexPositions = vertexPositions;
	this->translation = translation;
	this->rotation = rotation;
	this->scaling = scaling;
    this->material = material;
    this->uboWorldMatrixBlock = uboWorldMatrixBlock;

	currentTRS = translation * rotation * scaling;
}

void const Model::draw(mat4 parentTRS, int renderingMode, GLuint lightCoeffsLocation, GLuint lightColorLocation) {
  if(material.texture != 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.texture);
  }
    glBindVertexArray(vao);
	currentTRS = parentTRS * translation * rotation * scaling;

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

float Model::IntersectsRay(glm::vec3 source, glm::vec3 direction)
{
	float intersection = std::numeric_limits<float>::infinity() * -1.0f;
	if (numberOfVertices > 0) {
		for (int i = 0; i != numberOfVertices; i += 3)
		{
			//Three corners of a triangle

			glm::vec3 p0 = currentTRS * glm::vec4(vertexPositions.at(i), 1.0f);
			glm::vec3 p1 = currentTRS * glm::vec4(vertexPositions.at(i + 1), 1.0f);
			glm::vec3 p2 = currentTRS * glm::vec4(vertexPositions.at(i + 2), 1.0f);

			//define a plane based on triangle vertices
			glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));


			//Check plane is facing us
			if (glm::dot(normal, direction) < 0) {
				float t = (glm::dot(normal, p0) - glm::dot(normal, source)) / glm::dot(normal, direction);
				//check collision happens forward in time
				if (t >= 0) {
					//Plane contains point, now what about the triangle?
					glm::vec3 intersectionInPlane = source + t * direction;

					//Projection onto a plane, we must pick a plane not perpendicular to triangle, to avoid collapsing the triangle into a single line
					//At least one of these is guaranteed to be non-perpendicular. All are centered at the origin
					glm::vec3 xNormal = glm::vec3(1, 0, 0);
					glm::vec3 yNormal = glm::vec3(0, 1, 0);
					glm::vec3 zNormal = glm::vec3(0, 0, 1);

					glm::vec2 a;
					glm::vec2 b;
					glm::vec2 c;
					glm::vec2 r; //Ray intersection point				

					if (glm::dot(xNormal, normal) != 0.0f)
					{ //Project on yz-plane
						a = glm::vec2(p0.y, p0.z);
						b = glm::vec2(p1.y, p1.z);
						c = glm::vec2(p2.y, p2.z);
						r = glm::vec2(intersectionInPlane.y, intersectionInPlane.z);
					}
					else if (glm::dot(yNormal, normal) != 0.0f)
					{ //Project on xz-plane
						a = glm::vec2(p0.z, p0.x);
						b = glm::vec2(p1.z, p1.x);
						c = glm::vec2(p2.z, p2.x);
						r = glm::vec2(intersectionInPlane.z, intersectionInPlane.x);
					}
					else
					{ //Project on xy-plane
						a = glm::vec2(p0.x, p0.y);
						b = glm::vec2(p1.x, p1.y);
						c = glm::vec2(p2.x, p2.y);
						r = glm::vec2(intersectionInPlane.x, intersectionInPlane.y);
					}
					float areaABC = glm::length(glm::cross(glm::vec3(b - a, 0.0f), glm::vec3(c - a, 0.0f))) / 2.0f;

					float areaABP = glm::length(glm::cross(glm::vec3(b - a, 0.0f), glm::vec3(r - a, 0.0f))) / 2.0f;
					float areaACP = glm::length(glm::cross(glm::vec3(r - a, 0.0f), glm::vec3(c - a, 0.0f))) / 2.0f;
					float areaBCP = glm::length(glm::cross(glm::vec3(b - r, 0.0f), glm::vec3(c - r, 0.0f))) / 2.0f;

					float alpha = areaBCP / areaABC;
					float beta = areaACP / areaABC;
					float gamma = areaABP / areaABC;

					if ((alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f) && (alpha + beta + gamma == 1.0f))
					{
						intersection = t;
					}
				}
			}
		}
	}
	else {
		for (vector<Model*>::iterator it = children.begin(); it != children.end(); ++it)
		{
			float temp = (*it)->IntersectsRay(source, direction);
			if (temp >= 0.0f && temp > intersection) {
				intersection = temp;
			}
		}
	}
	return intersection;
}

