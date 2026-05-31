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

    // IDs de texturas cargadas en GPU
    GLuint trollTexID = 0;
    GLuint rockTexID = 0;

    void Run();

private:
    // Lighting constants
    const float AmbientStrength = 0.15f;
    const glm::vec3 LigthSource = glm::vec3(0.0f, 0.0f, 1.0f);
    const glm::vec3 LigthColor = glm::vec3( 1.0f,  1.0f,  1.0f);

    void Init();
    void RunLoop();
    void Update();
    void Render();
    void Stop();
    //USaremos para crear los objectos y assignar sus shaders
    GameObject* CreateGameObject(Mesh* mesh, const char* vertexShader, const char* geometryShader, const char* fragmentShader);
};