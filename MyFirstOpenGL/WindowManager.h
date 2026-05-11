#pragma once
#include <vector>
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <GL/glew.h>
class WindowManager
{
private: 
	void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight);
public:
	void initialize();
	void SetSettings();
	GLFWwindow* window;
};