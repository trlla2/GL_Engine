#include "TimeManager.h"
#include <GLFW/glfw3.h>

void TimeManager::Init()
{
	globalTime = 0.0f;
	scaleTime = 1.0f;
	lastFrameTime = 0.0f;
	deltaTime = 0.0f;
}

void TimeManager::Update()
{
	float currentFrameTime = static_cast<float>(glfwGetTime());
	deltaTime = (currentFrameTime - lastFrameTime) * scaleTime;
	lastFrameTime = currentFrameTime;
	globalTime += deltaTime;
}