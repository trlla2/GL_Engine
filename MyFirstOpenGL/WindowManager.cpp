#include "WindowManager.h"
#include "Game.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Referencia estática para acceder a Game desde el callback de redimensionado
static Game* game = nullptr;

void WindowManager::Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);

	for (size_t i = 0; i < game->compiledPrograms.size(); i++) {
		glUseProgram(game->compiledPrograms[i]);
		glUniform2f(glGetUniformLocation(game->compiledPrograms[i], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
	}
}

void WindowManager::Init(Game* currentGame)
{
	game = currentGame;

	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, Resize_Window);

	glfwMakeContextCurrent(window);
}

void WindowManager::SetSettings()
{
	glewExperimental = GL_TRUE;

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}
