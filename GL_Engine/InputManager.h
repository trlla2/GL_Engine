#pragma once
#include <GLFW/glfw3.h>

#define IM InputManager::GetInstance()

enum KeyState { RELEASED, DOWN, HOLD, UP };

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
	double GetMouseDeltaX();
	double GetMouseDeltaY();

private:
	InputManager() = default;
	InputManager(InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	~InputManager() {}

	GLFWwindow* currentWindow = nullptr;

	// El estado de cada tecla se deduce comparando el frame actual con el anterior
	bool currentKeys[GLFW_KEY_LAST + 1] = { false };
	bool previousKeys[GLFW_KEY_LAST + 1] = { false };

	double mouseX = 0.0, mouseY = 0.0;
	double mouseDeltaX = 0.0, mouseDeltaY = 0.0;
	bool firstMouse = true;
};
