#include "InputManager.h"
#include <iostream>

void InputManager::Init(GLFWwindow* window)
{
	currentWindow = window;
}

bool InputManager::Listen()
{
	if (!currentWindow)
	{
		std::cerr << "INPUT MANAGER: window not assigned" << std::endl;
		return false;
	}

	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; i++)
	{
		previousKeys[i] = currentKeys[i];
		currentKeys[i] = glfwGetKey(currentWindow, i) == GLFW_PRESS;
	}

	return true;
}

bool InputManager::GetEventKey(int key, KeyState event)
{
	if (key < GLFW_KEY_SPACE || key > GLFW_KEY_LAST)
	{
		std::cerr << "INPUT MANAGER: key out of range" << std::endl;
		return false;
	}
	if (!currentWindow)
	{
		std::cerr << "INPUT MANAGER: window not assigned" << std::endl;
		return false;
	}

	bool isCurrentDown = currentKeys[key];
	bool isPreviousDown = previousKeys[key];

	if (event == DOWN) return isCurrentDown && !isPreviousDown;
	if (event == HOLD) return isCurrentDown && isPreviousDown;
	if (event == UP) return !isCurrentDown && isPreviousDown;
	if (event == RELEASED) return !isCurrentDown && !isPreviousDown;

	return false;
}