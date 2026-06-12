#pragma once
#include <glm.hpp>
#include <GL/glew.h>

class Flashlight 
{
public:
	float innerCutOff = 10.f;
	float outerCutoff = 15.f;


	float intensity = 4.f;
	float range = 30.f;
	

	void HandleInput();
	void Update(GLuint* compiledProgram, glm::vec3 position,glm::vec3 forwardDirection);


private:
	bool active = true;
};

