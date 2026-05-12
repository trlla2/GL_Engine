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
	// Calcular nueva posicion orbital
    transform.position.x = pos.x + distance * glm::cos(glm::radians(angle));
    transform.position.z = pos.z + distance * glm::sin(glm::radians(angle));
	transform.position.y = pos.y + 1.f;

	// Establecer el punto de enfoque
	targetPosition = pos + glm::vec3(0.f, 1.0f, 0.f);
}

void Camera::DollyZoom(glm::vec3 pos)
{

    if (!dollyZoomActive)
    {
        dollyZoomActive = true;
        dollyZoomTimer = 0.f;
        dollyTarget = pos;

        // Guardar la distancia inicial al objetivo
        float distance = glm::length(transform.position - pos);

        // FOV inicial: usar el actual
        // La relacion que mantiene el dolly zoom: distance * tan(fov/2) = constante
        // => al reducir fov, acercamos la camara para compensar
    }

    if (dollyZoomActive)
    {
        dollyZoomTimer += TM->GetDeltaTime();
        float t = dollyZoomTimer / dollyZoomDuration; // 0..1

        // Interpolar FOV de 45 a 5 grados y volver (ida y vuelta suave)
        float targetFov = glm::mix(45.f, 5.f, glm::smoothstep(0.f, 1.f, t));

        // Mantener la altura percibida del objeto: distance * tan(fov/2) = K
        float K = glm::length(transform.position - dollyTarget) * glm::tan(glm::radians(fFov / 2.f));
        float newDist = K / glm::tan(glm::radians(targetFov / 2.f));

        // Mover camara a lo largo del eje hacia el objetivo
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
