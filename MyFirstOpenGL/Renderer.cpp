#include "Renderer.h"
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "TimeManager.h"

// Funciones auxiliares de matrices
glm::mat4 GenerateTranslationMatrix(glm::vec3 translation) {
    return glm::translate(glm::mat4(1.0f), translation);
}

glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees) {
    return glm::rotate(glm::mat4(1.0f), glm::radians(fDegrees), glm::normalize(axis));
}

glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis) {
    return glm::scale(glm::mat4(1.0f), scaleAxis);
}


void Renderer::RenderObject(const GameObject& obj, const Mesh& mesh, GLuint programId, int vertexCount)
{
    //Indicar a la tarjeta GPU que programa debe usar
    glUseProgram(programId);

    glm::mat4 translationMatrix = GenerateTranslationMatrix(obj.transform.position);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    if (glm::length(obj.transform.rotation) > 0.0001f) {
        rotationMatrix = GenerateRotationMatrix(obj.transform.rotation, glm::length(obj.transform.rotation));
    }
    
    glm::mat4 scaleMatrix = GenerateScaleMatrix(obj.transform.scale);

    // Pasar matrices al shader
    glUniformMatrix4fv(glGetUniformLocation(programId, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(programId, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(programId, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));

    // Dibujar
    mesh.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
    mesh.Unbind();
}

void Renderer::RenderPyramid(const GameObject& obj, const Mesh& mesh, GLuint programId, int vertexCount)
{
    glUseProgram(programId);

    //Solo cambia esto
    int colorId = (int)(TM->GetGlobalTime() * 0.5) % 3;
    glUniform1i(glGetUniformLocation(programId, "colorId"), colorId);

    // Calcular matrices de transformación
    glm::mat4 translationMatrix = GenerateTranslationMatrix(obj.transform.position);
    
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    if (glm::length(obj.transform.rotation) > 0.0001f) {
        rotationMatrix = GenerateRotationMatrix(obj.transform.rotation, glm::length(obj.transform.rotation));
    }
    
    glm::mat4 scaleMatrix = GenerateScaleMatrix(obj.transform.scale);

    // Pasar matrices al shader
    glUniformMatrix4fv(glGetUniformLocation(programId, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(programId, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(programId, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));

    // Dibujar
    mesh.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    mesh.Unbind();
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

