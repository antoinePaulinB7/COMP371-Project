#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>
#include "Model.h"


class Skybox : Model {
public: 
	Skybox(int vao, std::vector<glm::vec3> vertexPositions, unsigned int uboWorldMatrixBlock, std::vector<Model*> children, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling, Material material);

	void const draw(glm::mat4 parentTRS, int renderingMode, GLuint lightCoeffsLocation, GLuint lightColorLocation) override;

};