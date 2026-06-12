#pragma once
#include "GameObjects.h"

class Flashlight
{
public:
	float intensity = 4.f;
	float range = 30.f;
	
	
	float cutOff = 12.5f;

	void HandleInput();
	void Update(GLuint* compiledProgram, glm::vec3 position,glm::vec3 forwardDirection);


private:
	bool active = true;
};

