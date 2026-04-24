#pragma once
#include <GL/glew.h>

class Mesh 
{
private:
    GLuint vao;
    GLuint vbo;
    int vertexCount;

public:
    Mesh(const GLfloat* vertices, size_t vertexDataSize, int vertexCount);
    ~Mesh();

    void Bind() const;
    void Unbind() const;
    int GetVertexCount() const;
};