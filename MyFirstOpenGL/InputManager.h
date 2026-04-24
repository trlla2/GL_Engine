#pragma once
#include <GLFW/glfw3.h>

#define IM InputManager::GetInstance()

class InputManager
{
public:
	static InputManager* GetInstance()
	{
		static InputManager instance;
		return &instance;
	}

	void Listen(GLFWwindow* window);

	bool GetEventKey(int key, int event);

private:
	InputManager() = default;
	InputManager(InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	~InputManager() {}


	GLFWwindow* currentWindow;
};