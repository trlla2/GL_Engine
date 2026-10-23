#include "ShaderLoaders.h"

GLuint VertexShaderLoadable::LoadVertexShader(const std::string& filePath)
{
	return LoadShader(filePath, GL_VERTEX_SHADER);
}

GLuint GeometryShaderLoadable::LoadGeometryShader(const std::string& filePath)
{
	return LoadShader(filePath, GL_GEOMETRY_SHADER);
}

GLuint FragmentShaderLoadable::LoadFragmentShader(const std::string& filePath)
{
	return LoadShader(filePath, GL_FRAGMENT_SHADER);
}

GLuint IShaderLoadable::LoadShader(const std::string& filePath, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	std::string sShaderCode = fileLoader.Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	glShaderSource(shader, 1, &cShaderSource, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success) {
		return shader;
	}
	else {
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(shader, logLength, nullptr, errorLog.data());

		const char* shaderTypeName = (shaderType == GL_VERTEX_SHADER) ? "vertex" : (shaderType == GL_GEOMETRY_SHADER) ? "geometry" : (shaderType == GL_FRAGMENT_SHADER) ? "fragment" : "unknown";

		std::cerr << "Error al cargar el " << shaderTypeName << " shader: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}
