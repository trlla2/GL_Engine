#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

#define IM InputManager::GetInstance()

enum KeyState { EMPTY, DOWN, UP, HOLD, RELEASED };

class InputManager
{
public:
	static InputManager* GetInstance()
	{
		static InputManager instance;
		return &instance;
	}

	void Init(GLFWwindow* window);

	bool Listen();

	bool GetEventKey(int key, KeyState event);

	

private:
	InputManager() = default;
	InputManager(InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	~InputManager() {}


	GLFWwindow* currentWindow;

	std::unordered_map<int, KeyState> keyReference;
};