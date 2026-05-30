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
#include "TextureLoader.h"

class Game
{
public:
    std::vector<GLuint> compiledPrograms;
    std::vector<GameObject*> sceneObjects;
    std::vector<Mesh*> meshes;

    WindowManager* windowManager;
    Renderer renderer;
    Camera camera;
    OBJLoader objLoader;
    TextureLoader textureLoader;

    // IDs de texturas cargadas en GPU
    GLuint trollTexID = 0;
    GLuint rockTexID = 0;

    void Run();

private:
    void Init();
    void RunLoop();
    void Update();
    void Render();
    void Stop();
    //USaremos para crear los objectos y assignar sus shaders
    GameObject* CreateGameObject(Mesh* mesh, const char* vertexShader, const char* geometryShader, const char* fragmentShader);
};