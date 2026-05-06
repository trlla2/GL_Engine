#pragma once
#include <glm.hpp>

class Camera 
{

	int minFov = 1, maxFov = 90;

	glm::vec3 position = glm::vec3(0.5f, 0.5f, 1.f);
	glm::vec3 localVectorUp = glm::vec3(0.f, 1.f, 0.f);

	float fFov = 45.f;
	float fNear = 0.1f;
	float fFar = 10.f;
};