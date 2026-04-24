#include "InputManager.h"
#include <iostream>

void InputManager::Listen(GLFWwindow* window)
{
	currentWindow = window;
}

bool InputManager::GetEventKey(int key, int event)
{
	if (!currentWindow)
	{
		std::cerr << "INPUT MANAGER: window not assigned" << std::endl;
		return false;
	}

	return glfwGetKey(currentWindow, key) == event;
}
