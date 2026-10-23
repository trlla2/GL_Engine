#include "InputManager.h"
#include <iostream>

void InputManager::Init(GLFWwindow* window)
{
	currentWindow = window;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	double newX, newY;
	glfwGetCursorPos(currentWindow, &newX, &newY);
	if (firstMouse)
	{
		mouseX = newX;
		mouseY = newY;
		firstMouse = false;
	}
	mouseDeltaX = newX - mouseX;
	mouseDeltaY = newY - mouseY;
	mouseX = newX;
	mouseY = newY;

	return true;
}

double InputManager::GetMouseDeltaX()
{
	return mouseDeltaX;
}

double InputManager::GetMouseDeltaY()
{
	return mouseDeltaY;
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
