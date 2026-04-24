#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "ShaderProgram.h"
#include "GameObjects.h"
#include "Mesh.h"
#include "Renderer.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

std::vector<GLuint> compiledPrograms;

//Timer
float globalTime = 0.0f;
float scaleTime = 1.0f;
float lastFrameTime = 0.0f;
float deltaTime = 0.0f;

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
	//Definir nuevo tamano del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
	
	// Actualizar uniform para todos los programas
	for (size_t i = 0; i < compiledPrograms.size(); i++) {
		glUseProgram(compiledPrograms[i]);
		glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
	}
}

void main() {
	//Definir semillas del rand según el tiempo
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

	//Asignamos función de callback para cuando el frame buffer es modificado
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
		Cube* cube = new Cube();
		Pyramid* pyramid = new Pyramid();
		Rect* rect = new Rect();

		//Compilar shaders
		ShaderProgram* cubeProgram = new ShaderProgram("CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
		ShaderProgram* rectProgram = new ShaderProgram("RectVertexShader.glsl", "RectGeometryShader.glsl", "RectFragmentShader.glsl");
		ShaderProgram* pyramidProgram = new ShaderProgram("PyramidVertexShader.glsl", "PyramidGeometryShader.glsl", "PyramidFragmentShader.glsl");

		compiledPrograms.push_back(cubeProgram->programId);
		compiledPrograms.push_back(rectProgram->programId);
		compiledPrograms.push_back(pyramidProgram->programId);

		//Definir vertices para cada geometria
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

		GLfloat pyramidVertices[] = {
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

		// Crear meshes usando la clase Mesh
		Mesh* cubeMesh = new Mesh(cubeVertices, sizeof(cubeVertices), 14);
		Mesh* rectMesh = new Mesh(rectVertices, sizeof(rectVertices), 14);
		Mesh* pyramidMesh = new Mesh(pyramidVertices, sizeof(pyramidVertices), 18);

		// Crear renderer
		Renderer renderer;

		//Definimos color para limpiar el buffer de color
		renderer.SetClearColor(0.f, 0.f, 0.f, 1.f);
		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Asignar valores iniciales al programa (windowSize uniforms)
		for (size_t i = 0; i < compiledPrograms.size(); i++) {
			glUseProgram(compiledPrograms[i]);
			glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
		}

		//Defino la transform inicial del gameobject (cubo)
		cube->transform.position = glm::vec3(-0.6f, 0.f, 0.f);
		cube->transform.rotation = glm::vec3(60.f, 0.f, 0.f);
		cube->transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);

		const float sinWidth = 0.7f;
		const float cubeRotationSpeed = 10.f;

		//Defino la transform inicial del gameobject (orthoedro)
		rect->transform.position = glm::vec3(0.f, 0.f, 0.f);
		rect->transform.rotation = glm::vec3(80.f, 0.f, 0.f);
		rect->transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);

		const float rectRotationSpeed = 20.f;
		bool isExpanding = true;

		//Defino la transform inicial del gameobject (piramide)
		pyramid->transform.position = glm::vec3(0.65f, 0.f, 0.f);
		pyramid->transform.rotation = glm::vec3(270.f, 0.f, 0.f);
		pyramid->transform.scale = glm::vec3(1.f, 1.f, 1.75f);

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
					lastKeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
				{
					showCube = !showCube;
					lastKeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
				{
					showRect = !showRect;
					lastKeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
				{
					showPyramid = !showPyramid;
					lastKeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				{
					scaleTime = (scaleTime != 0.f) ? 0.f : 1.f;
					lastKeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
				{
					scaleTime += 0.1f;
					lastKeyPressTime = static_cast<float>(glfwGetTime());
				}

				if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
				{
					scaleTime -= 0.1f;
					lastKeyPressTime = static_cast<float>(glfwGetTime());
				}
			}



			//Codigo generao con Claude 4.5, 
			//Prompt: add a a timer counter, we will count the deltatime between frames and add it to a global time variable
			float currentFrameTime = static_cast<float>(glfwGetTime());
			deltaTime = (currentFrameTime - lastFrameTime) * scaleTime;
			lastFrameTime = currentFrameTime;
			globalTime += deltaTime;

			// Actualizar y renderizar cubo
			if (showCube)
			{
				cube->transform.position.y = sin(globalTime) * sinWidth;
				cube->transform.rotation.y += cubeRotationSpeed * deltaTime;
				renderer.RenderObject(*cube, *cubeMesh, compiledPrograms[0], 14);
			}

			// Actualizar y renderizar rect
			if (showRect)
			{
				rect->transform.rotation.z += rectRotationSpeed * deltaTime;

				// Expandir/contraer cada 3 segundos
				isExpanding = (int)globalTime % 6 < 3;
				if (isExpanding)
				{
					rect->transform.scale += glm::vec3(0.3f, 0.0f, -0.4f) * deltaTime;
				}
				else
				{
					rect->transform.scale -= glm::vec3(0.3f, 0.0f, -0.4f) * deltaTime;
				}

				renderer.RenderObject(*rect, *rectMesh, compiledPrograms[1], 14);
			}

			// Actualizar y renderizar piramide
			if (showPyramid)
			{
				pyramid->transform.position.y = sin(globalTime) * sinWidthPyramid;
				pyramid->transform.rotation.y += pyramidRotationSpeed * deltaTime;
				pyramid->transform.rotation.x += pyramidRotationSpeed * deltaTime;
				renderer.RenderPyramid(*pyramid, *pyramidMesh, compiledPrograms[2], 18);
			}

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