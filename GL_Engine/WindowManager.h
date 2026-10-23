#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Game;

class WindowManager
{
public:
    GLFWwindow* window = nullptr;

    WindowManager() { }
    void Init(Game* currentGame);
    void SetSettings();

private:
    // GLFW requiere que el callback sea una función estática
    static void Resize_Window(GLFWwindow* window, int width, int height);
};
