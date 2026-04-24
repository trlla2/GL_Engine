#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

std::vector<GLuint> compiledPrograms;

//Timer
float globalTime = 0.0f;
float scaleTime = 1.0f;
float lastFrameTime = 0.0f;
float deltaTime = 0.0f;



class Transform {
public :
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.f);
};

class GameObject {

public:	
	Transform transform;
	GameObject(GLuint VAO, GLuint VBO) {
		vao = VAO;
		vbo = VBO;
	}
private:
	GLuint vao = 0;
	GLuint vbo = 0;
};

class Camera {
	public:
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
};

class cube : public GameObject
{
	GLfloat vertices[42] = {
		-0.5f, 0.5f, -0.5f, // 3
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
		-0.5f, +0.5f, +0.5f  // 1
	};

	cube(GLuint VAO, GLuint VBO) : GameObject(VAO, VBO) {}
};

class pyramid : public GameObject
{
	GLfloat vertices[42] = {
		-0.5f, 0.5f, -0.5f, // 3
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
		+0.5f, -0.5f, +0.5f  // 4
	};
	pyramid(GLuint VAO, GLuint VBO) : GameObject(VAO, VBO) {}
};

class rect : public GameObject
{
	GLfloat vertices[42] = {
		-0.5f, 0.5f, -0.5f, // 3
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
		+0.5f, -0.5f, +0.5f  // 4
	};
	rect(GLuint VAO, GLuint VBO) : GameObject(VAO, VBO) {}
};


void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {


	//Definir nuevo tamano del viewport
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


void RenderGameObject(GameObject obj, GLuint vao ,int programIndex, int vertexPoints = 14) {
	//Indicar a la tarjeta GPU que programa debe usar
	glUseProgram(compiledPrograms[programIndex]);

	glm::mat4 translationMatrix = MatrixOperations.GenerateTranslationMatrix(obj.position);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	if (glm::length(obj.rotation) > 0.0001f) {
		rotationMatrix = GenerateRotationMatrix(obj.rotation, glm::length(obj.rotation));
	}
	glm::mat4 scaleMatrix = GenerateScaleMatrix(obj.scale);

	if (programIndex == 2)
	{
		//Solo pirmaide
		int colorId = 0;
		colorId = (int)(globalTime * 0.5) % 3; // Cambia el color cada 2 segundos entre 3 colores
		glUniform1i(glGetUniformLocation(compiledPrograms[2], "colorId"), colorId);
	}

	//Pasamos las variables al shader
	glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[programIndex], "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
	glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[programIndex], "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
	glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[programIndex], "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));

	//Definimos que queremos usar el VAO con los puntos
	glBindVertexArray(vao);

	//Definimos que queremos dibujar
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexPoints);

	//Dejamos de usar el VAO indicado anteriormente
	glBindVertexArray(0);
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
		cube.position = glm::vec3(-0.6f, 0.f, 0.f);
		cube.rotation = glm::vec3(60.f, 0.f, 0.f); //cooler looking starting angle
		cube.scale = glm::vec3(0.5f, 0.5f, 0.5f);

		const float sinWidth = 0.7f;
		const float cubeRotaionSpeed = 10.f;

		//Defino la transform inical del gameobject (ortodero)
		rect.position = glm::vec3(0.f, 0.f, 0.f);
		rect.rotation = glm::vec3(80.f, 0.f, 0.f); //cooler looking starting angle
		rect.scale = glm::vec3(0.5f, 0.5f, 0.5f);


		const float rectTargetScale = 1.f;
		const float rectRotationSpeed = 20.f;
		bool isExpanding = true;

		//Defino la transform inical del gameobject (piramide)
		pyramid.position = glm::vec3(0.65f, 0.f, 0.f);
		pyramid.rotation = glm::vec3(270.f, 0.f, 0.f);
		pyramid.scale = glm::vec3(1.f, 1.f, 1.75f);

		const float sinWidthPyramid = 0.5f;
		const float pyramidRotationSpeed = 35.f;

		float lastkeyPressTime = 0.f;
		const float delayBetweenKeyPresses = 0.2f; // 200 ms delay

		// Input variables
		bool showWireframe = false;

		bool showCube = true;
		bool showRect = true;
		bool showPyramid = true;

		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


			//INPUT 
			if (lastkeyPressTime + delayBetweenKeyPresses < static_cast<float>(glfwGetTime()) )
			{
				if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
				{
					showWireframe = !showWireframe;
					glPolygonMode(GL_FRONT_AND_BACK, showWireframe ? GL_LINE : GL_FILL);
					lastkeyPressTime = static_cast<float>(glfwGetTime());
				}

				if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
				{
					showCube = !showCube;
					lastkeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
				{
					showRect = !showRect;
					lastkeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
				{
					showPyramid = !showPyramid;
					lastkeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				{
					if(scaleTime != 0.f)
						scaleTime = 0.f;
					else
						scaleTime = 1.f;
					lastkeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
				{
					scaleTime += 0.1f;
					lastkeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
				{
					scaleTime -= 0.1f;
					lastkeyPressTime = static_cast<float>(glfwGetTime());
				}
			}



			//Codigo generao con Claude 4.5, 
			//Prompt: add a a timer counter, we will count the deltatime between frames and add it to a global time variable
			float currentFrameTime = static_cast<float>(glfwGetTime());
			deltaTime = (currentFrameTime - lastFrameTime) * scaleTime;
			lastFrameTime = currentFrameTime;
			globalTime += deltaTime;

			//Mover el cubo segun el tiempo
			cube.position.y = sin(globalTime) * sinWidth;

			//Rotar en Y indefinidamente
			cube.rotation.y += cubeRotaionSpeed * deltaTime;

			if(showCube)
				RenderGameObject(cube, cubeVao, 1);

			

			//Definimos que queremos usar el VAO con los puntos
			glBindVertexArray(cubeVao);

			//Definimos que queremos dibujar
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);

			//Dejamos de usar el VAO indicado anteriormente
			glBindVertexArray(0);


			//Indicar a la tarjeta GPU que programa debe usar
			glUseProgram(compiledPrograms[1]);

			//rota z indefinidamente
			rect.rotation.z += rectRotationSpeed * deltaTime;

			//A ojo podemos observar que estos valores son acceptables para lo que queremos vislauizar
			//basados en pura intuición, el cub se expanidirá y contraerá cada 3 segundos
			isExpanding = (int)globalTime % 6 < 3;

			if (isExpanding)
			{
				rect.scale += glm::vec3(0.3f, 0.0f, -0.4f) * deltaTime; // Escala de crecimiento menos en y para que escale como ortoedro que es 
			}
			else
			{
				rect.scale -= glm::vec3(0.3f, 0.0f, -0.4f) * deltaTime; 
			}

			if(showRect)
				RenderGameObject(rect, rectVao, 1);


			//Mover el cubo segun el tiempo
			pyramid.position.y = sin(globalTime) * sinWidthPyramid;

			//Rotar en X e Y indefinidamente
			pyramid.rotation.y += pyramidRotationSpeed * deltaTime;
			pyramid.rotation.x += pyramidRotationSpeed * deltaTime;

			if(showPyramid)
				RenderGameObject(pyramid, vaoPiramide, 2, 18);

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