#pragma once
#include <GL/glew.h>

class Mesh 
{
private:
    int vertexCount;

public:
    Mesh(const GLfloat* vertices, size_t vertexDataSize, int vertexCount);
    ~Mesh();

    GLuint vao;
    GLuint vbo;

    void Bind() const;
    void Unbind() const;
    int GetVertexCount() const;
};