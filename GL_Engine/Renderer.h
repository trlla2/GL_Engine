#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "Mesh.h"
#include "GameObjects.h"

class Renderer {
public:
    void RenderObject(const GameObject& obj, const Mesh& mesh, GLuint programId, int vertexCount,
                      GLuint texID, glm::vec4 tint);
    void SetClearColor(float r, float g, float b, float a);
    void Clear();
};
