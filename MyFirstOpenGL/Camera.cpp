#include "Camera.h"
#include "InputManager.h"
#include "TimeManager.h"
#include <gtc/type_ptr.hpp>


Camera::Camera()
{
    transform.position = glm::vec3(0.f, 1.f, 4.f);
}

void Camera::Update(GLuint* compiledProgram, int const windowWidth, int const windowHeight)
{
    
    yaw += (float)IM->GetMouseDeltaX() * mouseSensitivity;
    pitch -= (float)IM->GetMouseDeltaY() * mouseSensitivity;

    // limitamos el pitch para que no voltee la camara
    if (pitch > 89.f) pitch = 89.f;
    if (pitch < -89.f) pitch = -89.f;

    // forward y right a partir de los angulos
    glm::vec3 forward;
    forward.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    forward.y = glm::sin(glm::radians(pitch));
    forward.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    forward = glm::normalize(forward);

    glm::vec3 right = glm::normalize(glm::cross(forward, localVectorUp));

    
    float velocity = moveSpeed * TM->GetDeltaTime();
    if (IM->GetEventKey(GLFW_KEY_W, DOWN) || IM->GetEventKey(GLFW_KEY_W, HOLD))
        transform.position += forward * velocity;
    if (IM->GetEventKey(GLFW_KEY_S, DOWN) || IM->GetEventKey(GLFW_KEY_S, HOLD))
        transform.position -= forward * velocity;
    if (IM->GetEventKey(GLFW_KEY_D, DOWN) || IM->GetEventKey(GLFW_KEY_D, HOLD))
        transform.position += right * velocity;
    if (IM->GetEventKey(GLFW_KEY_A, DOWN) || IM->GetEventKey(GLFW_KEY_A, HOLD))
        transform.position -= right * velocity;

    lookAtPoint = transform.position + forward;

    glm::mat4 viewMatrix = glm::lookAt(transform.position, lookAtPoint, localVectorUp);
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fFov), (float)(windowWidth) / (float)(windowHeight), fNear, fFar);

    glUseProgram(*compiledProgram);
    glUniformMatrix4fv(glGetUniformLocation(*compiledProgram, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(*compiledProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUseProgram(0);
}

glm::vec3 Camera::GetForwardVector()
{
    return glm::normalize(lookAtPoint - transform.position);
}
