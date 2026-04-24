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
#include "GameObjects.h"
#include "Mesh.h"
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

	if (glewInit() == GLEW_OK)
	{

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

		// Crear meshes usando la clase Mesh
		Mesh* cubeMesh = new Mesh(cube->vertices, sizeof(cube->vertices), 14);
		Mesh* rectMesh = new Mesh(rect->vertices, sizeof(rect->vertices), 14);
		Mesh* pyramidMesh = new Mesh(pyramid->vertices, sizeof(pyramid->vertices), 18);

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

		// Input variables
		bool showWireframe = false;

		bool showCube = true;
		bool showRect = true;
		bool showPyramid = true;

		//Generamos el game loop

		// Activamos el Input Manager con la ventana 
		IM->Init(window);

		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			// Updateamos el Input Manager
			IM->Listen();

			// Updateamos el Time Manager
			TM->Update();



			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


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

			if (IM->GetEventKey(GLFW_KEY_3, KeyState::DOWN))
			{
				showRect = !showRect;
			}

			if (IM->GetEventKey(GLFW_KEY_4, KeyState::DOWN))
			{
				showPyramid = !showPyramid;
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
			cube->transform.rotation.y += cube->rotationSpeed* TM->GetDeltaTime();

			if (showCube)
				renderer.RenderObject(*cube, *cubeMesh, compiledPrograms[0]);


			//Indicar a la tarjeta GPU que programa debe usar
			glUseProgram(compiledPrograms[1]);

			//rota z indefinidamente
			rect->transform.rotation.z += rectRotationSpeed * TM->GetDeltaTime();

			//A ojo podemos observar que estos valores son acceptables para lo que queremos vislauizar
			//basados en pura intuición, el cub se expanidirá y contraerá cada 3 segundos
			isExpanding = (int)TM->GetGlobalTime() % 6 < 3;

			if (isExpanding)
			{
				rect->transform.scale += glm::vec3(0.3f, 0.0f, -0.4f) * TM->GetDeltaTime(); // Escala de crecimiento menos en y para que escale como ortoedro que es 
			}
			else
			{
				rect->transform.scale -= glm::vec3(0.3f, 0.0f, -0.4f) * TM->GetDeltaTime();

				// Actualizar y renderizar rect
				if (showRect)
				{
					rect->transform.rotation.z += rectRotationSpeed * TM->GetDeltaTime();

					// Expandir/contraer cada 3 segundos
					isExpanding = (int)TM->GetGlobalTime() % 6 < 3;
					if (isExpanding)
					{
						rect->transform.scale += glm::vec3(0.3f, 0.0f, -0.4f) * TM->GetDeltaTime();
					}
					else
					{
						rect->transform.scale -= glm::vec3(0.3f, 0.0f, -0.4f) * TM->GetDeltaTime();
					}
					// Actualizar y renderizar piramide
					if (showPyramid)
					{
						pyramid->transform.position.y = sin(TM->GetGlobalTime()) * sinWidthPyramid;
						pyramid->transform.rotation.y += pyramidRotationSpeed * TM->GetDeltaTime();
						pyramid->transform.rotation.x += pyramidRotationSpeed * TM->GetDeltaTime();
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
		}
		else
		{
			std::cout << "Ha petao." << std::endl;
			glfwTerminate();
		}

	//Finalizamos GLFW
	glfwTerminate();
}