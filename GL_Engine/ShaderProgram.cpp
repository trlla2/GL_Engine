#include "ShaderProgram.h"

GLuint ShaderProgram::CreateProgram()
{
	GLuint program = glCreateProgram();

	if (vertexShader != 0) {
		glAttachShader(program, vertexShader);
	}

	if (geometryShader != 0) {
		glAttachShader(program, geometryShader);
	}

	if (fragmentShader != 0) {
		glAttachShader(program, fragmentShader);
	}

	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (success) {
		// Una vez enlazado, los shaders ya no son necesarios en el programa
		if (vertexShader != 0) {
			glDetachShader(program, vertexShader);
		}

		if (geometryShader != 0) {
			glDetachShader(program, geometryShader);
		}

		if (fragmentShader != 0) {
			glDetachShader(program, fragmentShader);
		}

		return program;
	}
	else {
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());

		std::cerr << "Error al linkar el programa:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}
