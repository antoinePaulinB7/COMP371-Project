#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>
#include "Models/Model.h"

using namespace std;
using namespace glm;

//Adapted from the provided collisions lab code by Zachary Lapointe
bool doRaycastCollision(mat4 cameraViewMatrix, std::vector<Model*> modelsWithCollisions) {
	glm::mat4 inverseView = glm::inverse(cameraViewMatrix);
	glm::vec3 rayOrigin = inverseView[3]; //the camera position
	glm::vec3 rayDirection = -1.0f * glm::normalize(inverseView[2]); //the camera lookat

	//Try intersecting ray with every model
	//We are shooting a ray from the camera, directly forward
	float closestIntersection = std::numeric_limits<float>::infinity();
	Model* intersectingModel = nullptr;
	for (vector<Model*>::iterator it = modelsWithCollisions.begin(); it < modelsWithCollisions.end(); ++it)
	{
		Model* currentTarget = *it;
		float intersectionPoint = currentTarget->IntersectsRay(rayOrigin, rayDirection);
		if (intersectionPoint >= 0.0f && intersectionPoint < closestIntersection) {
			closestIntersection = intersectionPoint;
			intersectingModel = currentTarget;
		}

	}

	if (intersectingModel != nullptr)
	{
		if (closestIntersection < 100.0f) {
			cout << closestIntersection << endl;
			return true;
		}
	}

	return false;
}