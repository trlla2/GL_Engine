#pragma once
#include "GameObjects.h"

class Camera 
{
public:
	Transform transform;
	float fNear = 0.1f;
	float fFar = 10.f;
	int minFov = 1, maxFov = 90;

	void Update(GLuint* compiledProgram, int const windowWidth, int const windowHeight);

	void RotateOnPosition(glm::vec3 pos, float distance);

	void DollyZoom(glm::vec3 pos);

private:
	glm::vec3 localVectorUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 targetPosition = glm::vec3(0.f);  // Punto al que mira la cámara
	float fFov = 45.f;

	float dollyZoomTimer = 0.f;        
	bool  dollyZoomActive = false;     
	float dollyZoomDuration = 3.f;     
	glm::vec3 dollyTarget;             
};


