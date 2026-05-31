#pragma once
#include "GameObjects.h"

class Flashlight
{
public:
	glm::vec3 color = glm::vec3(1.f, 1.f, 0.5f);
	float intensity = 4.f;
	float range = 30.f;
	
	
	float innerCutOff = 12.5f;
	float outerCutOff = 17.5f;

	// detecta la tecla y cambia el estado, se llama una vez por frame
	void HandleInput();
	void Update(GLuint* compiledProgram, glm::vec3 position,glm::vec3 forwardDirection);


private:
	bool active = true;
};

