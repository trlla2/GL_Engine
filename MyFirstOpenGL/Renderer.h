#include "GameObject"
#include <GL/glew.h>

#pragma once
class Renderer {
public:
    void RenderObject(const GameObject& obj, const Mesh& mesh, GLuint programId, int vertexCount = 14);
    void RenderPyramid(const GameObject& obj, const Mesh& mesh, GLuint programId, int vertexCount = 18);
    void SetClearColor(float r, float g, float b, float a);
    void Clear();
};