#pragma once
#include "GameObjects.h"

class Camera
{
public:
	Camera();

	Transform transform;
	float fNear = 0.1f;
	float fFar = 100.f;

	void Update(GLuint* compiledProgram, int const windowWidth, int const windowHeight);
	glm::vec3 GetForwardVector();

private:
	glm::vec3 localVectorUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 lookAtPoint = glm::vec3(0.f, 1.f, 0.f);

	float fFov = 45.f;

	float yaw = -90.f;   
	float pitch = -6.f;
	float mouseSensitivity = 0.025f;
	float moveSpeed = 1.5f;
};
