#include "Mesh.h"
#include <GL/glew.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<float>& normals) : vertexCount(vertices.size() / 3), vboUVs(0), vboNormals(0)
{
    //Almaceno la cantidad de vertices que habra
    this->vertexCount = vertices.size() / 3;

    //Generamos VAO/VBO
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    //Defino el VAO creado como activo
    glBindVertexArray(this->vao);

    //Defino el VBO de las posiciones como activo, le paso los datos y lo configuro
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //Activamos el atributo 0 (posiciones por defecto)
    glEnableVertexAttribArray(0);

    // Registrar UVs en location 1
    if (!uvs.empty())
    {
        glGenBuffers(1, &this->vboUVs);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboUVs);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    // Registrar normales en location 2
    if (!normals.empty())
    {
        glGenBuffers(1, &this->vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboNormals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
    }

    //Desvinculamos VAO y VBO
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

    // Dibujamos
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