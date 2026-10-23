#pragma once
#include <string>
#include <GL/glew.h>
#include "ShaderLoaders.h"

class ShaderProgram
{
private:
	VertexShaderLoadable vertexShaderLoader;
	GeometryShaderLoadable geometryShaderLoader;
	FragmentShaderLoadable fragmentShaderLoader;

	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;

	GLuint CreateProgram();

public:
	GLuint programId = 0;

	ShaderProgram(std::string vertexFileName, std::string geometryFileName, std::string fragmentFilename)
	{
		vertexShader = vertexShaderLoader.LoadVertexShader(vertexFileName);
		geometryShader = geometryShaderLoader.LoadGeometryShader(geometryFileName);
		fragmentShader = fragmentShaderLoader.LoadFragmentShader(fragmentFilename);

		programId = CreateProgram();
	}
};
