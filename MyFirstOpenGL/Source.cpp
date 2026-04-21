#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

std::vector<GLuint> compiledPrograms;

//Timer
float globalTime = 0.0f;
float lastFrameTime = 0.0f;
float deltaTime = 0.0f;

//Struct para controlar el GameObject (cubo)
struct GameObject {

	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.f);
};

struct ShaderProgram {

	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};


void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {

	//Definir nuevo tama�o del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
	glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);

}

//Funcion que genera una matriz de escalado representada por un vector
glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis) {

	return glm::scale(glm::mat4(1.0f), scaleAxis);
}

//Funcion que genera una matriz de rotacion dado un angulo y un vector
glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees) {

	return glm::rotate(glm::mat4(1.0f), glm::radians(fDegrees), glm::normalize(axis));
}

//Funcion que genera una matriz de traslacion representada por un vector
glm::mat4 GenerateTranslationMatrix(glm::vec3 translation) {

	return glm::translate(glm::mat4(1.0f), translation);
}

//Funcion que devolvera una string con todo el archivo leido
std::string Load_File(const std::string& filePath) {

	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	//Lanzamos error si el archivo no se ha podido abrir
	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Leemos el contenido y lo volcamos a la variable auxiliar
	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	//Cerramos stream de datos y devolvemos contenido
	file.close();

	return fileContent;
}

GLuint LoadFragmentShader(const std::string& filePath) {

	// Crear un fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Usamos la funcion creada para leer el fragment shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el fragment shader con su c�digo fuente
	glShaderSource(fragmentShader, 1, &cShaderSource, nullptr);

	// Compilar el fragment shader
	glCompileShader(fragmentShader);

	// Verificar errores de compilaci�n
	GLint success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el fragment shader
	if (success) {

		return fragmentShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(fragmentShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el fragment shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


GLuint LoadGeometryShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su c�digo fuente
	glShaderSource(geometryShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(geometryShader);

	// Verificar errores de compilaci�n
	GLint success;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return geometryShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(geometryShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GLuint LoadVertexShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su c�digo fuente
	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(vertexShader);

	// Verificar errores de compilaci�n
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return vertexShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//Funci�n que dado un struct que contiene los shaders de un programa generara el programa entero de la GPU
GLuint CreateProgram(const ShaderProgram& shaders) {

	//Crear programa de la GPU
	GLuint program = glCreateProgram();

	//Verificar que existe un vertex shader y adjuntarlo al programa
	if (shaders.vertexShader != 0) {
		glAttachShader(program, shaders.vertexShader);
	}

	if (shaders.geometryShader != 0) {
		glAttachShader(program, shaders.geometryShader);
	}

	if (shaders.fragmentShader != 0) {
		glAttachShader(program, shaders.fragmentShader);
	}

	// Linkear el programa
	glLinkProgram(program);

	//Obtener estado del programa
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	//Devolver programa si todo es correcto o mostrar log en caso de error
	if (success) {

		//Liberamos recursos
		if (shaders.vertexShader != 0) {
			glDetachShader(program, shaders.vertexShader);
		}

		//Liberamos recursos
		if (shaders.geometryShader != 0) {
			glDetachShader(program, shaders.geometryShader);
		}

		//Liberamos recursos
		if (shaders.fragmentShader != 0) {
			glDetachShader(program, shaders.fragmentShader);
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

void main() {

	//Definir semillas del rand seg�n el tiempo
	srand(static_cast<unsigned int>(time(NULL)));

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos funci�n de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	glEnable(GL_CULL_FACE);

	//Indicamos lado del culling
	glCullFace(GL_BACK);

	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {

		//Declarar instancia de GameObject
		GameObject cube;
		GameObject pyramid;
		GameObject rect;

		//Compilar shaders
		ShaderProgram cubeProgram;
		cubeProgram.vertexShader = LoadVertexShader("CubeVertexShader.glsl");
		cubeProgram.geometryShader = LoadGeometryShader("CubeGeometryShader.glsl");
		cubeProgram.fragmentShader = LoadFragmentShader("CubeFragmentShader.glsl");

		ShaderProgram rectProgram;
		rectProgram.vertexShader = LoadVertexShader("RectVertexShader.glsl");
		rectProgram.geometryShader = LoadGeometryShader("RectGeometryShader.glsl");
		rectProgram.fragmentShader = LoadFragmentShader("RectFragmentShader.glsl");

		ShaderProgram pyramidProgram;
		pyramidProgram.vertexShader = LoadVertexShader("PyramidVertexShader.glsl");
		pyramidProgram.geometryShader = LoadGeometryShader("PyramidGeometryShader.glsl");
		pyramidProgram.fragmentShader = LoadFragmentShader("PyramidFragmentShader.glsl");

		//Compilar programa
		compiledPrograms.push_back(CreateProgram(cubeProgram));

		//Definimos color para limpiar el buffer de color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		GLuint cubeVao, cubeVbo;

		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &cubeVao);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(cubeVao);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &cubeVbo);

		//Indico que el VBO activo es el que acabo de crear y que almacenar� un array. Todos los VBO que genere se asignaran al �ltimo VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);

		//Posici�n X e Y del punto
		GLfloat cubeVertices[] = {
			-0.5f, +0.5f, -0.5f, // 3
			+0.5f, +0.5f, -0.5f, // 2
			-0.5f, -0.5f, -0.5f, // 6
			+0.5f, -0.5f, -0.5f, // 7
			+0.5f, -0.5f, +0.5f, // 4
			+0.5f, +0.5f, -0.5f, // 2
			+0.5f, +0.5f, +0.5f, // 0
			-0.5f, +0.5f, -0.5f, // 3
			-0.5f, +0.5f, +0.5f, // 1
			-0.5f, -0.5f, -0.5f, // 6
			-0.5f, -0.5f, +0.5f, // 5
			+0.5f, -0.5f, +0.5f, // 4
			-0.5f, +0.5f, +0.5f, // 1
			+0.5f, +0.5f, +0.5f  // 0
		};

		

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como esta distribuida la informaci�n
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		//Indicamos que la tarjeta gr�fica puede usar el atributo 0
		glEnableVertexAttribArray(0);

		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAO
		glBindVertexArray(0);

		// Orthohedron (rect)
		compiledPrograms.push_back(CreateProgram(rectProgram));

		GLuint rectVao, rectVbo;

		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &rectVao);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(rectVao);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &rectVbo);

		//Indico que el VBO activo es el que acabo de crear y que almacenar� un array. Todos los VBO que genere se asignaran al �ltimo VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, rectVbo);

		//Posici�n X e Y del punto
		GLfloat rectVertices[] = {
			-0.5f, +0.5f, -0.5f, // 3
			+0.5f, +0.5f, -0.5f, // 2
			-0.5f, -0.5f, -0.5f, // 6
			+0.5f, -0.5f, -0.5f, // 7
			+0.5f, -0.5f, +0.5f, // 4
			+0.5f, +0.5f, -0.5f, // 2
			+0.5f, +0.5f, +0.5f, // 0
			-0.5f, +0.5f, -0.5f, // 3
			-0.5f, +0.5f, +0.5f, // 1
			-0.5f, -0.5f, -0.5f, // 6
			-0.5f, -0.5f, +0.5f, // 5
			+0.5f, -0.5f, +0.5f, // 4
			-0.5f, +0.5f, +0.5f, // 1
			+0.5f, +0.5f, +0.5f  // 0
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Piramide 

		compiledPrograms.push_back(CreateProgram(pyramidProgram));
		GLuint vaoPiramide, vboPiramide;
		glGenVertexArrays(1, &vaoPiramide);
		glBindVertexArray(vaoPiramide);

		glGenBuffers(1, &vboPiramide);
		glBindBuffer(GL_ARRAY_BUFFER, vboPiramide);

		GLfloat pyramidVertices[] = { // He utilizado la ia (claude Haiku 4.5) Prompt (con contexto del codigo): Hazme los puntos de una priramide 3d de forma que la pueda dibujar con un solo glDrawArray
			0.3f, -0.3f, 0.0f,
			0.0f,  0.0f, 0.5f,
			-0.3f, -0.3f, 0.0f,

			0.3f,  0.3f, 0.0f,
			0.0f,  0.0f, 0.5f,
			0.3f, -0.3f, 0.0f,

			-0.3f,  0.3f, 0.0f,
			0.0f,  0.0f, 0.5f,
			0.3f,  0.3f, 0.0f,

			-0.3f, -0.3f, 0.0f,
			0.0f,  0.0f, 0.5f,
			-0.3f,  0.3f, 0.0f,

			-0.3f,  0.3f, 0.0f,
			-0.3f, -0.3f, 0.0f,
			0.3f, -0.3f, 0.0f,

			0.3f, -0.3f, 0.0f,
			0.3f,  0.3f, 0.0f,
			-0.3f,  0.3f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		//Indicar a la tarjeta GPU que programa debe usar
		glUseProgram(compiledPrograms[0]);

		//Asignar valores iniciales al programa
		glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

		glUseProgram(compiledPrograms[1]);
		glUniform2f(glGetUniformLocation(compiledPrograms[1], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

		glUseProgram(compiledPrograms[2]);
		glUniform2f(glGetUniformLocation(compiledPrograms[2], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

		//Defino la transform inical del gameobject (cubo)
		cube.position = glm::vec3(-0.5f, 0.f, 0.f);
		cube.rotation = glm::vec3(60.f, 0.f, 0.f); //cooler looking starting angle
		cube.scale = glm::vec3(0.5f, 0.5f, 0.5f);

		const float sinWidth = 0.7f;

		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Codigo generao con Claude 4.5, 
			//Prompt: add a a timer counter, we will count the deltatime between frames and add it to a global time variable
			float currentFrameTime = static_cast<float>(glfwGetTime());
			deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;
			globalTime += deltaTime;

			//Mover el cubo segun el tiempo
			cube.position.y = sin(globalTime) * sinWidth;

			//Rotar en Y indefinidamente
			cube.rotation.y += 5.f * deltaTime;

			//Genero matrices de transformacion (cubo)
			glm::mat4 translationMatrix = GenerateTranslationMatrix(cube.position);
			glm::mat4 rotationMatrix = glm::mat4(1.0f);
			if (glm::length(cube.rotation) > 0.0001f) {
				rotationMatrix = GenerateRotationMatrix(cube.rotation, glm::length(cube.rotation));
			}
			glm::mat4 scaleMatrix = GenerateScaleMatrix(cube.scale);

			

			//Pasamos las variables al shader
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));

			

			//Definimos que queremos usar el VAO con los puntos
			glBindVertexArray(cubeVao);

			//Definimos que queremos dibujar
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);

			//Dejamos de usar el VAO indicado anteriormente
			glBindVertexArray(0);


			//Indicar a la tarjeta GPU que programa debe usar
			glUseProgram(compiledPrograms[1]);
			// Orthohedron (long cube)

			rect.position = glm::vec3(0.f, 0.f, 0.f);
			rect.rotation = glm::vec3(0.f, 0.f, 0.f);
			rect.scale = glm::vec3(0.5f, 0.75f, 0.5f);

			glm::mat4 rectTraslation = GenerateTranslationMatrix(rect.position);
			glm::mat4 rectRotation = glm::mat4(1.0f);
			if (glm::length(rect.rotation) > 0.0001f) {
				rectRotation = GenerateRotationMatrix(rect.rotation, glm::length(rect.rotation));
			}
			glm::mat4 rectScale = GenerateScaleMatrix(rect.scale);

			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[1], "translationMatrix"), 1, GL_FALSE, glm::value_ptr(rectTraslation));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[1], "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rectRotation));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[1], "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(rectScale));

			

			glBindVertexArray(cubeVao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
			glBindVertexArray(0);

			//Indicar a la tarjeta GPU que programa debe usar
			glUseProgram(compiledPrograms[2]);

			// pyramid
			pyramid.position = glm::vec3(0.65f, 0.f, 0.0f);
			pyramid.rotation = glm::vec3(-90.f, 0.f, 0.f);
			pyramid.scale = glm::vec3(0.75f, .75f, 1.5f);

			glm::mat4 pyramidTranslation = GenerateTranslationMatrix(pyramid.position);
			glm::mat4 pyramidRotation = glm::mat4(1.0f);
			if (glm::length(pyramid.rotation) > 0.0001f) {
				pyramidRotation = GenerateRotationMatrix(pyramid.rotation, glm::length(pyramid.rotation));
			}
			glm::mat4 pyramidScale = GenerateScaleMatrix(pyramid.scale);

			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[2], "translationMatrix"), 1, GL_FALSE, glm::value_ptr(pyramidTranslation));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[2], "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(pyramidRotation));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[2], "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(pyramidScale));

			glBindVertexArray(vaoPiramide);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
			glBindVertexArray(0);

			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

		//Desactivar y eliminar programa
		glUseProgram(0);
		glDeleteProgram(compiledPrograms[0]);

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();

}