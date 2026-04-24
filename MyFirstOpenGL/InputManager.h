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

private:
	InputManager() = default;
	InputManager(InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	~InputManager() {}

	GLFWwindow* currentWindow = nullptr;

	// GEMINI  (antes tenia un map con las ints de las keys con los states) prompt: engo este Input manager que no funciona bien cuando una tecla, cuando se pressiona(en un frame), cuando se mantiene presionada(todos los frames presionada), cuando se levanta(frame que se levanta), y cuando esta liberada o sin presionar.siempre esta detectando.AHORA CUANDO HAGO UN GETKEYEVENT DOWN SIEMPRE ESTA DANDO TRUE CUANDO SOLO DEVERIA DAR TRUE EL PRIMER FRAME
	
	// EL CODIGO NO ESTA SIMPLEMENTE COPIADO, EL GEMINI SOLO HA SIDO UN APOYO PARA ENTENDER EL PROBLEMA Y COMO SOLUCIONARLO
	
	bool currentKeys[GLFW_KEY_LAST + 1] = { false };
	bool previousKeys[GLFW_KEY_LAST + 1] = { false };
};