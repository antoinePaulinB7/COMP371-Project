#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>
#include "Model.h"

using namespace std;
using namespace glm;

//Adapted from the provided collisions lab code by Zachary Lapointe
void doRaycastCollision(vec3 cameraPosition, vec3 cameraLookAt, std::vector<Model*> modelsWithCollisions) {
	glm::vec3 rayDirection = -1.0f * glm::normalize(cameraLookAt); //the camera lookat

	//Try intersecting ray with every model
	//We are shooting a ray from the camera, directly forward
	float closestIntersection = std::numeric_limits<float>::infinity();
	Model* intersectingModel = nullptr;
	for (vector<Model*>::iterator it = modelsWithCollisions.begin(); it < modelsWithCollisions.end(); ++it)
	{
		Model* currentTarget = *it;
		float intersectionPoint = currentTarget->IntersectsRay(cameraPosition, rayDirection);
		if (intersectionPoint >= 0.0f && intersectionPoint < closestIntersection) {
			closestIntersection = currentTarget->IntersectsRay(cameraPosition, rayDirection);
			intersectingModel = currentTarget;
		}

	}

	if (intersectingModel != nullptr)
	{
		glm::vec3 intersectingPoint = cameraPosition + closestIntersection * rayDirection;
		glm::vec3 force = 5.0f * glm::normalize(intersectingModel->GetPosition() - intersectingPoint);
		intersectingModel->Accelerate(force, dt);

		glm::vec3 torque = 2.0f * (glm::cross(glm::normalize(force), glm::normalize(intersectingPoint)));
		intersectingModel->Angulate(torque, dt);
	}
}