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

	glUniform3f(glGetUniformLocation(*compiledProgram, "spotPosition"), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(*compiledProgram, "spotDirection"), forwardDirection.x, forwardDirection.y, forwardDirection.z);
	
	glUniform1f(glGetUniformLocation(*compiledProgram, "spotInnerCutOff"), glm::cos(glm::radians(innerCutOff)));
	glUniform1f(glGetUniformLocation(*compiledProgram, "spotOuterCutOff"), glm::cos(glm::radians(outerCutoff)));




	glUseProgram(0);
}
