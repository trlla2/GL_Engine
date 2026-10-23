#include "Mesh.h"
#include <GL/glew.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<float>& normals) : vertexCount(vertices.size() / 3), vboUVs(0), vboNormals(0)
{
    this->vertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    glBindVertexArray(this->vao);

    // Location 0: posiciones
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Location 1: UVs
    if (!uvs.empty())
    {
        glGenBuffers(1, &this->vboUVs);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboUVs);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    // Location 2: normales
    if (!normals.empty())
    {
        glGenBuffers(1, &this->vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboNormals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vbo);
    if (vboUVs != 0) glDeleteBuffers(1, &vboUVs);
    if (vboNormals != 0) glDeleteBuffers(1, &vboNormals);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::Render() const
{
    this->Bind();
    glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
    this->Unbind();
}

void Mesh::Bind() const
{
    glBindVertexArray(vao);
}

void Mesh::Unbind() const
{
    glBindVertexArray(0);
}

int Mesh::GetVertexCount() const
{
    return vertexCount;
}
