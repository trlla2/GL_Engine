#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "InputManager.h"
#include "TimeManager.h"
#include "ShaderProgram.h"
#include "Renderer.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

std::vector<GLuint> compiledPrograms;

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
	//Definir nuevo tamano del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
	
	// Actualizar uniform para todos los programas
	for (size_t i = 0; i < compiledPrograms.size(); i++) {
		glUseProgram(compiledPrograms[i]);
		glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
	}
}



struct Camera {

	int minFov = 1, maxFov = 90;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);  
	glm::vec3 localVectorUp = glm::vec3(0.f, 1.f, 0.f);

	float fFov = 45.f;
	float fNear = 0.1f;
	float fFar = 10.f;
};

void main() 
{
	// Inicializar el gestor de tiempo
	TM->Init();

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

	//Habilitamos depth testing
	glEnable(GL_DEPTH_TEST);

	if (glewInit() == GLEW_OK)
	{

		//Declarar instancia de GameObject
		Cube* cube = new Cube();

		Camera camera;

		//Compilar shaders
		ShaderProgram* cubeProgram = new ShaderProgram("CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");

		compiledPrograms.push_back(cubeProgram->programId);

		// Crear meshes usando la clase Mesh
		Mesh* cubeMesh = new Mesh(cube->vertices, sizeof(cube->vertices), 14);

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

		bool isExpanding = true;


		// Input variables
		bool showWireframe = false;

		bool showCube = true;

		//Generamos el game loop

		// Activamos el Input Manager con la ventana 
		IM->Init(window);

		while (!glfwWindowShouldClose(window))
		{

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			// Updateamos el Input Manager
			IM->Listen();

			// Updateamos el Time Manager
			TM->Update();



			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Calcular matrices de cámara
			glm::mat4 viewMatrix = glm::lookAt(camera.position, camera.position + glm::vec3(0.f, 0.f, -1.f), camera.localVectorUp);
			camera.fFov = camera.fFov < camera.minFov ? camera.minFov : camera.fFov;
			camera.fFov = camera.fFov > camera.maxFov ? camera.maxFov : camera.fFov;
			glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fFov), (float)(WINDOW_WIDTH) / (float)(WINDOW_HEIGHT), camera.fNear, camera.fFar);

			//Enviar matrices al shader
			glUseProgram(compiledPrograms[0]);
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

			//INPUT 
			if (IM->GetEventKey(GLFW_KEY_1, KeyState::DOWN))
			{
				showWireframe = !showWireframe;
				glPolygonMode(GL_FRONT_AND_BACK, showWireframe ? GL_LINE : GL_FILL);
			}

			if (IM->GetEventKey(GLFW_KEY_2, KeyState::DOWN))
			{
				showCube = !showCube;
			}


			if (IM->GetEventKey(GLFW_KEY_SPACE, KeyState::DOWN))
			{
				if (TM->GetScaleTime() != 0.f)
					TM->SetScaleTime(0.f);
				else
					TM->SetScaleTime(1.f);
			}

			if (IM->GetEventKey(GLFW_KEY_M, KeyState::DOWN))
			{
				TM->SetScaleTime(TM->GetScaleTime() + 0.1f);
			}

			if (IM->GetEventKey(GLFW_KEY_N, KeyState::DOWN))
			{
				TM->SetScaleTime(TM->GetScaleTime() - 0.1f);
			}

			//Mover el cubo segun el tiempo
			cube->transform.position.y = sin(TM->GetGlobalTime()) * sinWidth;

			//Rotar en Y indefinidamente
			cube->transform.rotation.y += cube->rotationSpeed * TM->GetDeltaTime();

			if (showCube)
				renderer.RenderObject(*cube, *cubeMesh, compiledPrograms[0], 14);
			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

	}
	else
	{
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();
}