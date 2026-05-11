#include "Camera.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

void Camera::Update(GLuint* compiledProgram, int const windowWidth, int const windowHeight)
{
	//Calcular matrices de cámara
	glm::mat4 viewMatrix = glm::lookAt(transform.position, transform.position + glm::vec3(0.f, 0.f, -1.f), localVectorUp);
	fFov = fFov < minFov ? minFov : fFov;
	fFov = fFov > maxFov ? maxFov : fFov;
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(fFov), (float)(windowWidth) / (float)(windowHeight), fNear, fFar);


	//Enviar matrices al shader
	glUseProgram(*compiledProgram);
	glUniformMatrix4fv(glGetUniformLocation(*compiledProgram, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(*compiledProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUseProgram(0);
}
