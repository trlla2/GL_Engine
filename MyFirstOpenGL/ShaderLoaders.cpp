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

	//Crear shader del tipo especificado
	GLuint shader = glCreateShader(shaderType);

	//Usamos la función para leer el shader y almacenarlo
	std::string sShaderCode = fileLoader.Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el shader con su código fuente
	glShaderSource(shader, 1, &cShaderSource, nullptr);

	//Compilar el shader
	glCompileShader(shader);

	//Verificar errores de compilación
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	//Si la compilación ha sido exitosa devolvemos el shader
	if (success) {
		return shader;
	}
	else {
		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(shader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		const char* shaderTypeName = (shaderType == GL_VERTEX_SHADER) ? "vertex" : (shaderType == GL_GEOMETRY_SHADER) ? "geometry" : (shaderType == GL_FRAGMENT_SHADER) ? "fragment" : "unknown";

		std::cerr << "Error al cargar el " << shaderTypeName << " shader: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}

}