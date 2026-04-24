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

	for (std::unordered_map<int, KeyState>::iterator it = keyReference.begin(); it != keyReference.end(); it++)
	{
		int key = it->first;
		int state = glfwGetKey(currentWindow, key);
		KeyState& keyState = keyReference[key];

		if (state == GLFW_PRESS)
		{
			if (keyState == EMPTY || keyState == UP || keyState == RELEASED)
				keyState = DOWN;
			else if (keyState == DOWN || keyState == HOLD)
				keyState = HOLD;
		}
		else if (state == GLFW_RELEASE)
		{
			if (keyState == DOWN || keyState == HOLD)
				keyState = UP;
			else if (keyState == UP || keyState == RELEASED)
				keyState = RELEASED;
		}
	}
	return true;
}

bool InputManager::GetEventKey(int key, KeyState event)
{
	if (key < 32 || key > GLFW_KEY_LAST)
	{
		std::cerr << "INPUT MANAGER: key out of range" << std::endl;
		return false;
	}
	if (!currentWindow)
	{
		std::cerr << "INPUT MANAGER: window not assigned" << std::endl;
		return false;
	}

	return glfwGetKey(currentWindow, key) == event;
}
