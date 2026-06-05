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

	glUniform3f(glGetUniformLocation(*compiledProgram, "spotlightPos"), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(*compiledProgram, "spotlightDir"), forwardDirection.x, forwardDirection.y, forwardDirection.z);

	glUniform3f(glGetUniformLocation(*compiledProgram, "spotlightColor"),color.x, color.y, color.z);
	glUniform1f(glGetUniformLocation(*compiledProgram, "spotlightIntensity"), currentIntensity);
	glUniform1f(glGetUniformLocation(*compiledProgram, "spotlightRange"), range);

	glUniform1f(glGetUniformLocation(*compiledProgram, "spotlightInnerCutOff"), glm::cos(glm::radians(innerCutOff)));
	glUniform1f(glGetUniformLocation(*compiledProgram, "spotlightOuterCutOff"), glm::cos(glm::radians(outerCutOff)));

	glUseProgram(0);
}
