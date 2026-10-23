#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "Mesh.h"
#include "Camera.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "GameObjects.h"
#include "OBJLoader.h"
#include "TextureLoader.h"
#include "Flashlight.h"
#include "Spawner.h"

class Game
{
public:
    std::vector<GLuint> compiledPrograms;
    std::vector<GameObject*> sceneObjects;
    std::vector<Mesh*> meshes;

    WindowManager* windowManager;
    Renderer renderer;
    Camera camera;
    Flashlight flashlight;
    OBJLoader objLoader;
    TextureLoader textureLoader;
    Spawner spawner;

    GLuint trollTexID = 0;
    GLuint rockTexID = 0;
    GLuint colormapTexID = 0;

    GameObject* SunGO = nullptr;
    GameObject* MoonGO = nullptr;

    void Run();

    GameObject* CreateGameObject(Mesh* mesh, const char* vertexShader, const char* geometryShader, const char* fragmentShader);

private:
    void Init();
    void RunLoop();
    void Update();
    void Render();
    void Stop();
};
