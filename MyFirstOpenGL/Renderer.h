#include <GL/glew.h>
#include "Mesh.h"
#include "GameObjects.h"
#pragma once
class Renderer {
public:
    void RenderObject(const GameObject& obj, const Mesh& mesh, GLuint programId, int vertexCount);;
    void SetClearColor(float r, float g, float b, float a);
    void Clear();
};