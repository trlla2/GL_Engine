#include <GL/glew.h>
#include <glm.hpp>
#include "Mesh.h"
#include "ShaderProgram.h"

class Transform {
public:
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.f);
};

class GameObject {

public:
	Transform transform;

	GameObject() = default;
};

class Cube : public GameObject
{

public:

	float rotationSpeed = 10.f;
	GLfloat vertices[42] = {
		-0.5f, +0.5f, -0.5f, // 3
		+0.5f, +0.5f, -0.5f, // 2
		-0.5f, -0.5f, -0.5f, // 6
		+0.5f, -0.5f, -0.5f, // 7
		+0.5f, -0.5f, +0.5f, // 4
		+0.5f, +0.5f, -0.5f, // 2
		+0.5f, +0.5f, +0.5f, // 0
		-0.5f, +0.5f, -0.5f, // 3
		-0.5f, +0.5f, +0.5f, // 1
		-0.5f, -0.5f, -0.5f, // 6
		-0.5f, -0.5f, +0.5f, // 5
		+0.5f, -0.5f, +0.5f, // 4
		-0.5f, +0.5f, +0.5f, // 1
		+0.5f, +0.5f, +0.5f  // 0
	};

	Cube() : GameObject() {}
};

class Pyramid : public GameObject
{
public:
	float rotationSpeed = 35.f;
	GLfloat vertices[54] = {
		0.3f, -0.3f, 0.0f,
		0.0f,  0.0f, 0.5f,
		-0.3f, -0.3f, 0.0f,

		0.3f,  0.3f, 0.0f,
		0.0f,  0.0f, 0.5f,
		0.3f, -0.3f, 0.0f,

		-0.3f,  0.3f, 0.0f,
		0.0f,  0.0f, 0.5f,
		0.3f,  0.3f, 0.0f,

		-0.3f, -0.3f, 0.0f,
		0.0f,  0.0f, 0.5f,
		-0.3f,  0.3f, 0.0f,

		-0.3f,  0.3f, 0.0f,
		0.3f,  -0.3f, 0.0f,
		-0.3f, -0.3f, 0.0f,

		0.3f,  -0.3f, 0.0f,
		-0.3f,  0.3f, 0.0f,
		0.3f,   0.3f, 0.0f
	};

	Pyramid() : GameObject() {}
};

class Rect : public GameObject
{
public:
	float rotationSpeed = 20.f;
	GLfloat vertices[42] = {
		-0.5f, +0.5f, -0.5f, // 3
		+0.5f, +0.5f, -0.5f, // 2
		-0.5f, -0.5f, -0.5f, // 6
		+0.5f, -0.5f, -0.5f, // 7
		+0.5f, -0.5f, +0.5f, // 4
		+0.5f, +0.5f, -0.5f, // 2
		+0.5f, +0.5f, +0.5f, // 0
		-0.5f, +0.5f, -0.5f, // 3
		-0.5f, +0.5f, +0.5f, // 1
		-0.5f, -0.5f, -0.5f, // 6
		-0.5f, -0.5f, +0.5f, // 5
		+0.5f, -0.5f, +0.5f, // 4
		-0.5f, +0.5f, +0.5f, // 1
		+0.5f, +0.5f, +0.5f  // 0
	};

	public: Rect() : GameObject() {}
};
