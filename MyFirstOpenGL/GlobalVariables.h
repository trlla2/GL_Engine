#pragma once
#include <vector>
#include <GL/glew.h>
#include "Mesh.h"

#define GV GlobalVariables::GetInstance()

class GlobalVariables 
{
public:
	static GlobalVariables& GetInstance() {
		static GlobalVariables instance;
		return instance;
	}

	std::vector<GLuint> compiledPrograms;
	std::vector<Mesh> meshes;
};