#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Fordward declaration pq sino el compilador llora por includes circulares
class Game;

class WindowManager
{
public:
    GLFWwindow* window = nullptr;

    WindowManager() { }
    void Init(Game* currentGame);
    void SetSettings();

private:
	//Esta funcion es especial i no va a cambiar, es un callback que se llama cada vez que redimensionamos la ventana, por eso es static
    static void Resize_Window(GLFWwindow* window, int width, int height);
};