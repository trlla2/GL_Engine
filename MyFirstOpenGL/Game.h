#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"
#include "Camera.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "GameObjects.h"
#include "OBJLoader.h"

class Game
{
public:
	//Variables globales (ya no son globales en si, yay!)
    std::vector<GLuint> compiledPrograms;
    std::vector<GameObject*> sceneObjects;
	std::vector<Mesh*> meshes;

    //Systemas separados
    WindowManager* windowManager;
    Renderer renderer;
    Camera camera;
	OBJLoader objLoader;

    void Run();

private:
    void Init();
    void RunLoop();
    void Update();
    void Render();
    void Stop();
};