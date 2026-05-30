#include "Game.h"
#include "ShaderProgram.h"
#include "InputManager.h"
#include "TimeManager.h"
#include <iostream>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

const glm::vec4 TINT_TROLL_GREEN = glm::vec4(0.4f, 1.0f, 0.4f, 1.0f);
const glm::vec4 TINT_TROLL_BLUE  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 TINT_TROLL_TEAL  = glm::vec4(0.4f, 0.9f, 0.8f, 1.0f);
const glm::vec4 TINT_ROCK_NORMAL = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 TINT_CLOUD       = glm::vec4(1.2f, 1.2f, 1.2f, 1.0f);

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

    // cargar texturas tras inicializar el contexto OpenGL
    trollTexID = textureLoader.Load("Assets/Texturas/troll.png");
    rockTexID = textureLoader.Load("Assets/Texturas/rock.png");

        //// Create camera
        //Camera camera;
        //camera.transform.position = glm::vec3(0.f, 0.5f, 4.f);
        //camera.transform.rotation = glm::vec3(0.f, 0.f, -50.f);
    // Cargar mallas (reutilizar en todas sus instancias)
    Mesh* trollMesh = objLoader.LoadOBJMesh("Assets/Modelos/troll.obj");
    meshes.push_back(trollMesh);
    Mesh* rockMesh = objLoader.LoadOBJMesh("Assets/Modelos/rock.obj");
    meshes.push_back(rockMesh);

    // Crear GameObjects (asignar tints y texturas)
    // Prompt IA: Crea los objetos necesarios que salen en el documento adjunto
    // --- Troll izquierdo (verde) ---
    GameObject* trollLeft = CreateGameObject(trollMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    trollLeft->transform.position = glm::vec3(-3.f, 0.f, 0.f);
    trollLeft->tint = TINT_TROLL_GREEN;
    trollLeft->texID = trollTexID;

    // --- Troll central (azul/original) ---
    GameObject* trollCenter = CreateGameObject(trollMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    trollCenter->transform.position = glm::vec3(0.f, 0.f, 0.f);
    trollCenter->tint = TINT_TROLL_BLUE;
    trollCenter->texID = trollTexID;

    // --- Troll derecho (verde-azulado) ---
    GameObject* trollRight = CreateGameObject(trollMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    trollRight->transform.position = glm::vec3(3.f, 0.f, 0.f);
    trollRight->tint = TINT_TROLL_TEAL;
    trollRight->texID = trollTexID;
    
    // --- Roca suelo og ---
    GameObject* rock = CreateGameObject(rockMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    rock->transform.position = glm::vec3(0.f, .5f, -2.f);
    rock->tint = TINT_ROCK_NORMAL;
    rock->texID = rockTexID;

    // --- Nube izquierda (flying rock deformada) ---
    GameObject* cloudLeft = CreateGameObject(rockMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    cloudLeft->transform.position = glm::vec3(-2.f, 5.f, -4.f);
    cloudLeft->transform.scale = glm::vec3(2.0f, 0.5f, 1.0f);
    cloudLeft->tint = TINT_CLOUD;
    cloudLeft->texID = rockTexID;

    // --- Nube derecha ---
    GameObject* cloudRight = CreateGameObject(rockMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    cloudRight->transform.position = glm::vec3(3.f, 5.f, -4.f);
    cloudRight->transform.scale = glm::vec3(1.5f, 0.4f, 1.0f);
    cloudRight->tint = TINT_CLOUD;
    cloudRight->texID = rockTexID;

    //Definimos color para limpiar el buffer de color
    renderer.SetClearColor(0.f, 0.f, 0.f, 1.f);

    //Definimos modo de dibujo para cada cara
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Asignar valores iniciales al shader
    for (size_t i = 0; i < compiledPrograms.size(); i++) {
        glUseProgram(compiledPrograms[i]);
        glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

        // Directional light
        glUniform3f(glGetUniformLocation(compiledPrograms[i], "ligthSource"), LigthSource.x, LigthSource.y, LigthSource.z);
        glUniform3f(glGetUniformLocation(compiledPrograms[i], "lightColor"),     LigthColor.x,     LigthColor.y,     LigthColor.z);

        // Ambient light
        glUniform1f(glGetUniformLocation(compiledPrograms[i], "ambientStrength"), AmbientStrength);
    }
}

void Game::Update()
{

    // el toggle se procesa una vez por frame, fuera del bucle
    flashlight.HandleInput();

    // actualizar las matrizes de la camara y flaslight
    for (size_t i = 0; i < compiledPrograms.size(); i++)
    {
        camera.Update(&compiledPrograms[i], WINDOW_WIDTH, WINDOW_HEIGHT);
        flashlight.Update(&compiledPrograms[i], camera.transform.position, camera.GetForwardVector());
    }
}

void Game::Render()
{
    // limpiar color y depth buffer para evitar z-fighting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // renderizar cada instancia con su textura y tint propios
    for (GameObject* obj : sceneObjects)
    {
        renderer.RenderObject(*obj, *obj->mesh, obj->shaderProgram->programId,
                              obj->mesh->GetVertexCount(), obj->texID, obj->tint);
    }

    //Cambiamos buffers
    glFlush();
    glfwSwapBuffers(windowManager->window);
}

void Game::Stop()
{
    // liberar texturas al cerrar
    glDeleteTextures(1, &trollTexID);
    glDeleteTextures(1, &rockTexID);

    for (GameObject* obj : sceneObjects)
        delete obj;

    sceneObjects.clear();
    glfwTerminate();
}

GameObject* Game::CreateGameObject(Mesh* mesh, const char* vertexShader, const char* geometryShader, const char* fragmentShader)
{
    //Compilar shaders
    ShaderProgram* shader = new ShaderProgram(vertexShader, geometryShader, fragmentShader);
    compiledPrograms.push_back(shader->programId);

	//Crear GO y poner en array
    GameObject* obj = new GameObject(mesh, shader);
    sceneObjects.push_back(obj);

    return obj;
}