#include "Camera.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "TimeManager.h"


void Camera::Update(GLuint* compiledProgram, int const windowWidth, int const windowHeight)
{

	//Calcular matrices de cámara usando lookAt directamente
	glm::mat4 viewMatrix = glm::lookAt(transform.position, targetPosition, localVectorUp);
	fFov = fFov < minFov ? minFov : fFov;
	fFov = fFov > maxFov ? maxFov : fFov;
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(fFov), (float)(windowWidth) / (float)(windowHeight), fNear, fFar);

	//Enviar matrices al shader
	glUseProgram(*compiledProgram);
	glUniformMatrix4fv(glGetUniformLocation(*compiledProgram, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(*compiledProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUseProgram(0);
}

void Camera::RotateOnPosition(glm::vec3 pos, float distance)
{
	static float angle = 0.f;
    angle += TM->GetDeltaTime() * 45.f;

    if (angle > 360.f)
		angle = 0.f;
	std::cout << "Camera angle: " << angle << std::endl;
	// Calcular nueva posicion 
    transform.position.x = pos.x + distance * glm::cos(glm::radians(angle));
    transform.position.z = pos.z + distance * glm::sin(glm::radians(angle));
	transform.position.y = pos.y + 1.f;

	
	targetPosition = pos + glm::vec3(0.f, 1.0f, 0.f);
}

void Camera::DollyZoom(glm::vec3 pos, float startDegrees, float finishDegrees)
{

    

    if (!dollyZoomActive)
    {
        dollyZoomActive = true;
        dollyZoomTimer = 0.f;
        dollyTarget = pos;
    }

    if (dollyZoomActive)
    {
        // actualizamos el tiempo
        dollyZoomTimer += TM->GetDeltaTime();
        float t = dollyZoomTimer / dollyZoomDuration;

        // AI PROMPT: hay alguna manera de hacer una interpolacion Opengl C++
        // Interpolar FOV
        float targetFov = glm::mix(startDegrees, finishDegrees, glm::smoothstep(0.f, 1.f, t));

		// calculamos nueva posicion 
        float K = glm::length(transform.position - dollyTarget) * glm::tan(glm::radians(fFov / 2.f));
        float newDist = K / glm::tan(glm::radians(targetFov / 2.f));

        // updateamos la posicion de la camara
        glm::vec3 dir = glm::normalize(transform.position - dollyTarget);
        transform.position = dollyTarget + dir * newDist;

        fFov = targetFov;
        if (dollyZoomTimer >= dollyZoomDuration)
        {
            dollyZoomActive = false;
            dollyZoomTimer = 0.f;
        }
    }
}
