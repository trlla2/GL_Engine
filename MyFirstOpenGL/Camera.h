#pragma once
#include "GameObjects.h"

enum CameraMode {
	ORBIT,
	DOLLY_ZOOM, 
	STATIC
};

class Camera 
{
public:
	Transform transform;
	float fNear = 0.1f;
	float fFar = 100.f;
	int minFov = 1, maxFov = 90;

	CameraMode mode = ORBIT;
	glm::vec3 targetPosition = glm::vec3(0.f, 0.f, 0.f);


	void Update(GLuint* compiledProgram, int const windowWidth, int const windowHeight);

	void RotateOnPosition(glm::vec3 pos, float distance = 4.f);

	void DollyZoom(glm::vec3 pos, float startDegrees = 45.f, float finishDegrees = 5.f);

private:
	glm::vec3 localVectorUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 lookAtPoint = glm::vec3(0.f);  // Punto al que mira la cámara
	float fFov = 45.f;

	float dollyZoomTimer = 0.f;        
	bool  dollyZoomActive = false;     
	float dollyZoomDuration = 3.f;     
	glm::vec3 dollyTarget;     
	float dollyInitialDist = 0.f;
};


