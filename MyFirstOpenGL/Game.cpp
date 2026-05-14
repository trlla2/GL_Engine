#include "Game.h"
#include "ShaderProgram.h"
#include "InputManager.h"
#include "TimeManager.h"
#include <iostream>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void Game::Run()
{
    Init();

	RunLoop();

    Stop();
}

void Game::RunLoop()
{
    while (!glfwWindowShouldClose(windowManager->window))
    {
        glfwPollEvents();
        IM->Listen();
        TM->Update();
        Update();
        Render();
    }
}

void Game::Init()
{
    // Inicializar el gestor de tiempo
    TM->Init();

    //Inicializamos GLFW para gestionar ventanas e inputs
    glfwInit();

    //Create windowmanager
	windowManager = new WindowManager();

    //Iniciamos estor de ventanas
    windowManager->Init(this);

	//Retonrmamos si no se ha podido iniciar GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Ha petao." << std::endl;
        glfwTerminate();
        return;
    }

	//Configuramos la ventana
    windowManager->SetSettings();

	//Activamos el Input Manager con la ventana que hemos creado
    IM->Init(windowManager->window);

    // Crear shader para el Troll
    ShaderProgram* trollShader = new ShaderProgram("CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    compiledPrograms.push_back(trollShader->programId);

    // Create camera
    Camera camera;
    camera.transform.position = glm::vec3(0.f, 0.5f, 4.f);
    camera.transform.rotation = glm::vec3(0.f, 0.f, 0.f);

    //Compilar shaders
    ShaderProgram* cubeProgram = new ShaderProgram("CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");

    compiledPrograms.push_back(cubeProgram->programId);


    // Cargar mesh del Troll
    Mesh* trollMesh = objLoader.LoadOBJMesh("Assets/Modelos/troll.obj");
    meshes.push_back(trollMesh);

    // Crear el Troll GameObject
    Troll* troll = new Troll(trollMesh, trollShader);

    //Definimos color para limpiar el buffer de color
    renderer.SetClearColor(0.f, 0.f, 0.f, 1.f);

    //Definimos modo de dibujo para cada cara
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Asignar valores iniciales al shader
    for (size_t i = 0; i < compiledPrograms.size(); i++) {
        glUseProgram(compiledPrograms[i]);
        glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}

void Game::Update()
{
    static bool showWireframe = false;

    // actualizar las matrizes de la camara
    camera.DollyZoom(troll->transform.position);

    camera.Update(&compiledPrograms[0], WINDOW_WIDTH, WINDOW_HEIGHT);


    //INPUT 
    if (IM->GetEventKey(GLFW_KEY_1, KeyState::DOWN))
    {
        showWireframe = !showWireframe;
        glPolygonMode(GL_FRONT_AND_BACK, showWireframe ? GL_LINE : GL_FILL);
    }

    if (IM->GetEventKey(GLFW_KEY_SPACE, KeyState::DOWN))
    {
        if (TM->GetScaleTime() != 0.f)
            TM->SetScaleTime(0.f);
        else
            TM->SetScaleTime(1.f);
    }

    if (IM->GetEventKey(GLFW_KEY_M, KeyState::DOWN))
    {
        TM->SetScaleTime(TM->GetScaleTime() + 0.1f);
    }

    if (IM->GetEventKey(GLFW_KEY_N, KeyState::DOWN))
    {
        TM->SetScaleTime(TM->GetScaleTime() - 0.1f);
    }

}

void Game::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Renderizar meshes de la escena
    for (GameObject* obj : sceneObjects)
    {
        renderer.RenderObject(*obj, *obj->mesh, obj->shaderProgram->programId, obj->mesh->GetVertexCount());
    }

    //Cambiamos buffers
    glFlush();
    glfwSwapBuffers(windowManager->window);
}

void Game::Stop()
{
    for (GameObject* obj : sceneObjects)
        delete obj;

    sceneObjects.clear();
    glfwTerminate();
}