#include "Camera.h"
#include <gtc/type_ptr.hpp>


void Camera::Update(GLuint* compiledProgram, int const windowWidth, int const windowHeight)
{
    //posicion fija de la camara
    transform.position = glm::vec3(0.f, 1.f, 4.f);
    lookAtPoint = glm::vec3(0.f, 1.f, 0.f);

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
