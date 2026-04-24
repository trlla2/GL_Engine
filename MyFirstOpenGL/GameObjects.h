#include <GL/glew.h>
#include <glm.hpp>
#include "Mesh.h"
#include "Renderer.h"
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

	void Render(Renderer& renderer) {};
};

class Camera {
public:
	Transform transform;
};

class Cube : public GameObject
{
	GLfloat vertices[42] = {
		-0.5f, 0.5f, -0.5f, // 3
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
		-0.5f, +0.5f, +0.5f  // 1
	};

	public: Cube() : GameObject() {}
};

class Pyramid : public GameObject
{
	GLfloat vertices[42] = {
		-0.5f, 0.5f, -0.5f, // 3
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
		+0.5f, -0.5f, +0.5f  // 4
	};
	public: Pyramid() : GameObject() {}
};

class Rect : public GameObject
{
	GLfloat vertices [54] = { // He utilizado la ia (claude Haiku 4.5) Prompt (con contexto del codigo): Hazme los puntos de una priramide 3d de forma que la pueda dibujar con un solo glDrawArray
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
			-0.3f, -0.3f, 0.0f,
			0.3f, -0.3f, 0.0f,

			0.3f, -0.3f, 0.0f,
			0.3f,  0.3f, 0.0f,
			-0.3f,  0.3f, 0.0f
	};

	public: Rect() : GameObject() {}
};