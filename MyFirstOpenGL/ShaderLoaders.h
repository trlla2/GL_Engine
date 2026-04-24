#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
	
#include "FileLoader.h"

class IShaderLoadable {
protected:
	FileLoader fileLoader;

	GLuint LoadShader(const std::string& filePath, GLenum shaderType);
};

class VertexShaderLoadable : public IShaderLoadable {
public:
	GLuint LoadVertexShader(const std::string& filePath);
};


class GeometryShaderLoadable : public IShaderLoadable {
public:
	GLuint LoadGeometryShader(const std::string& filePath);
};

class FragmentShaderLoadable : public IShaderLoadable {
public:
	GLuint LoadFragmentShader(const std::string& filePath);
};

