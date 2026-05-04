#include "Mesh.h"
#include <GL/glew.h>

Mesh::Mesh(const GLfloat* vertices, size_t vertexDataSize, int vertexCount)  : vertexCount(vertexCount)
{
    ///Definimos cantidad de vao a crear y donde almacenarlos 
	glGenVertexArrays(1, &vao);

	//Indico que el VAO activo de la GPU es el que acabo de crear
	glBindVertexArray(vao);

	//Definimos cantidad de vbo a crear y donde almacenarlos
	glGenBuffers(1, &vbo);

	//Indico que el VBO activo es el que acabo de crear y que almacenar� un array. Todos los VBO que genere se asignaran al ultimo VAO que he hecho glBindVertexArray
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Ponemos los valores en el VBO creado
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertices, GL_STATIC_DRAW);

	//Indicamos donde almacenar y como esta distribuida la informaci�n
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Indicamos que la tarjeta grafica puede usar el atributo 0
	glEnableVertexAttribArray(0);

	//Desvinculamos VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Desvinculamos VAO
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::Bind() const
{
    glBindVertexArray(vao);
}

void Mesh::Unbind() const
{
    glBindVertexArray(0);
}