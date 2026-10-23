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
    glm::vec4 tint = glm::vec4(1.f, 1.f, 1.f, 1.f);
    GLuint texID = 0;
    bool enableLighting = true;

    Mesh* mesh;
    ShaderProgram* shaderProgram;

    GameObject(Mesh* mesh, ShaderProgram* shaderProgram) : mesh(mesh), shaderProgram(shaderProgram) {}
};

class Troll : public GameObject {
public:
    Troll(Mesh* mesh, ShaderProgram* shaderProgram) : GameObject(mesh, shaderProgram) {}
};

class Rock : public GameObject {
public:
    Rock(Mesh* mesh, ShaderProgram* shaderProgram) : GameObject(mesh, shaderProgram) {}
};

// El sol y la luna son emisivos: no reciben iluminación
class Sun : public GameObject
{
public:
    Sun(Mesh* mesh, ShaderProgram* shaderProgram) : GameObject(mesh, shaderProgram)
    {
        enableLighting = false;
    }
};

class Moon : public GameObject
{
public:
    Moon(Mesh* mesh, ShaderProgram* shaderProgram) : GameObject(mesh, shaderProgram)
    {
        enableLighting = false;
    }
};
