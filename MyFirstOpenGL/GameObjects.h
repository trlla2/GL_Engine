#pragma once

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

	Mesh* mesh;
	ShaderProgram* shaderProgram;

	GameObject(Mesh* mesh, ShaderProgram* shaderProgram) : mesh(mesh), shaderProgram(shaderProgram) {}
	~GameObject() 
	{
		delete mesh;
		delete shaderProgram;
	}
};

class Troll : public GameObject {
public:
	Troll(Mesh* mesh, ShaderProgram* shaderProgram) : GameObject(mesh, shaderProgram) {}
};