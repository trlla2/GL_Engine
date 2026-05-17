#include "WindowManager.h"
#include "Game.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//Pointer Statico para poder acceder a la clase Game desde el callback de redimensionado,
static Game* game = nullptr;

void WindowManager::Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
	//Definir nuevo tamano del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);

	// Actualizar uniform para todos los programas
	for (size_t i = 0; i < game->compiledPrograms.size(); i++) {
		glUseProgram(game->compiledPrograms[i]);
		glUniform2f(glGetUniformLocation(game->compiledPrograms[i], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
	}
}

void WindowManager::Init(Game* currentGame)
{
	//Seteamos nuestro pointer al gmae q nos han pasado
	game = currentGame;

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos función de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

}

void WindowManager::SetSettings()
{
	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	glEnable(GL_CULL_FACE);
	//Activamos depth test
	glEnable(GL_DEPTH_TEST);

	//Indicamos lado del culling
	glCullFace(GL_BACK);
}
