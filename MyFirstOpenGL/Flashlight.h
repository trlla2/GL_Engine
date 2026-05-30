#pragma once
#include "GameObjects.h"

class Flashlight
{
public:
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	float intensity = 1.f;
	float range = 30.f;
	
	float innerCutOff = 12.5f;
	float outerCutOff = 17.5f;

	void Update(GLuint* compiledProgram);

private:
	Transform transform;

};

