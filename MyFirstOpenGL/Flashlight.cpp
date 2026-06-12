#include "Flashlight.h"
#include "InputManager.h"

void Flashlight::HandleInput()
{
	if (IM->GetEventKey(GLFW_KEY_F, KeyState::DOWN))
		active = !active;
}

void Flashlight::Update(GLuint* compiledProgram, glm::vec3 position, glm::vec3 forwardDirection)
{
	glUseProgram(*compiledProgram);

	
	float currentIntensity = active ? intensity : 0.f;


	glUseProgram(0);
}
