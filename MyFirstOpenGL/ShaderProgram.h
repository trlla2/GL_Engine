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

	ShaderProgram(std::string vertexFileName, std::string  geometryFileName, std::string fragmentFilename)
	{
		vertexShader = vertexShaderLoader.LoadVertexShader("CubeVertexShader.glsl");
		geometryShader = geometryShaderLoader.LoadGeometryShader("CubeGeometryShader.glsl");
		fragmentShader = fragmentShaderLoader.LoadFragmentShader("CubeFragmentShader.glsl");

		CreateProgram();
	}

	GLuint CreateProgram(){}
};

GLuint ShaderProgram::CreateProgram()
{
	//Crear programa de la GPU
	GLuint program = glCreateProgram();

	//Verificar que existe un vertex shader y adjuntarlo al programa
	if (vertexShader != 0) {
		glAttachShader(program, vertexShader);
	}

	if (geometryShader != 0) {
		glAttachShader(program, geometryShader);
	}

	if (fragmentShader != 0) {
		glAttachShader(program, fragmentShader);
	}

	// Linkear el programa
	glLinkProgram(program);

	//Obtener estado del programa
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	//Devolver programa si todo es correcto o mostrar log en caso de error
	if (success) {

		//Liberamos recursos
		if (vertexShader != 0) {
			glDetachShader(program, vertexShader);
		}

		//Liberamos recursos
		if (geometryShader != 0) {
			glDetachShader(program, geometryShader);
		}

		//Liberamos recursos
		if (fragmentShader != 0) {
			glDetachShader(program, fragmentShader);
		}

		return program;
	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		//Almacenamos log
		std::vector<GLchar> errorLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());

		std::cerr << "Error al linkar el programa:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}