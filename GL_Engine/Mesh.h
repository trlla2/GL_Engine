#pragma once
#include <GL/glew.h>
#include <vector>

class Mesh
{
private:
    int vertexCount;
    GLuint vao;
    GLuint vbo;
    GLuint vboUVs;
    GLuint vboNormals;

public:
    Mesh(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
    void Render() const;
    ~Mesh();

    void Bind() const;
    void Unbind() const;
    int GetVertexCount() const;
};
