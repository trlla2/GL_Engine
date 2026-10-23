#include "Game.h"
#include "ShaderProgram.h"
#include "InputManager.h"
#include "TimeManager.h"
#include <gtc/constants.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stb_image.h>

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
    TM->Init();

    glfwInit();

    windowManager = new WindowManager();
    windowManager->Init(this);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error: no se ha podido inicializar GLEW." << std::endl;
        glfwTerminate();
        return;
    }

    windowManager->SetSettings();

    IM->Init(windowManager->window);

    // Las texturas se cargan una vez creado el contexto de OpenGL
    trollTexID = textureLoader.Load("Assets/Texturas/troll.png");
    rockTexID = textureLoader.Load("Assets/Texturas/rock.png");
    colormapTexID = textureLoader.Load("Assets/Texturas/colormap.png");

    // Se lee la fila central de Ambient.png como gradiente del ciclo día/noche
    int width = 0, height = 0, channels = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("Assets/Texturas/Ambient.png", &width, &height, &channels, 0);
    if (data && width > 0)
    {
        int targetY = height / 2;
        for (int x = 0; x < width; ++x)
        {
            int index = (targetY * width + x) * channels;
            float r = data[index] / 255.0f;
            float g = (channels > 1) ? data[index + 1] / 255.0f : r;
            float b = (channels > 2) ? data[index + 2] / 255.0f : r;
            ambientColors.push_back(glm::vec3(r, g, b));
        }
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Game: failed to load Ambient.png pixels!" << std::endl;
        abort();
    }

    // Las mallas se cargan una sola vez y se comparten entre instancias
    Mesh* trollMesh = objLoader.LoadOBJMesh("Assets/Modelos/troll.obj");
    meshes.push_back(trollMesh);
    Mesh* rockMesh = objLoader.LoadOBJMesh("Assets/Modelos/rock.obj");
    meshes.push_back(rockMesh);

    Mesh* hayBaleMesh = objLoader.LoadOBJMesh("Assets/Modelos/hay-bale.obj");
    meshes.push_back(hayBaleMesh);
    Mesh* pineFallMesh = objLoader.LoadOBJMesh("Assets/Modelos/pine-fall.obj");
    meshes.push_back(pineFallMesh);
    Mesh* trunkMesh = objLoader.LoadOBJMesh("Assets/Modelos/trunk.obj");
    meshes.push_back(trunkMesh);
    Mesh* pineMesh = objLoader.LoadOBJMesh("Assets/Modelos/pine.obj");
    meshes.push_back(pineMesh);

    // El sol y la luna usan la misma malla: el loader solo admite caras trianguladas
    Mesh* sunMesh = objLoader.LoadOBJMesh("Assets/Modelos/Sun.obj");
    meshes.push_back(sunMesh);
    Mesh* moonMesh = objLoader.LoadOBJMesh("Assets/Modelos/Sun.obj");
    meshes.push_back(moonMesh);

    GameObject* trollLeft = CreateGameObject(trollMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    trollLeft->transform.position = glm::vec3(-3.f, 0.f, 0.f);
    trollLeft->tint = TINT_TROLL_GREEN;
    trollLeft->texID = trollTexID;

    GameObject* trollCenter = CreateGameObject(trollMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    trollCenter->transform.position = glm::vec3(0.f, 0.f, 0.f);
    trollCenter->tint = TINT_TROLL_BLUE;
    trollCenter->texID = trollTexID;

    GameObject* trollRight = CreateGameObject(trollMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    trollRight->transform.position = glm::vec3(3.f, 0.f, 0.f);
    trollRight->tint = TINT_TROLL_TEAL;
    trollRight->texID = trollTexID;

    GameObject* rock = CreateGameObject(rockMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    rock->transform.position = glm::vec3(0.f, .5f, -2.f);
    rock->tint = TINT_ROCK_NORMAL;
    rock->texID = rockTexID;

    // Nubes: rocas escaladas de forma no uniforme
    GameObject* cloudLeft = CreateGameObject(rockMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    cloudLeft->transform.position = glm::vec3(-2.f, 5.f, -4.f);
    cloudLeft->transform.scale = glm::vec3(2.0f, 0.5f, 1.0f);
    cloudLeft->tint = TINT_CLOUD;
    cloudLeft->texID = rockTexID;

    GameObject* cloudRight = CreateGameObject(rockMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    cloudRight->transform.position = glm::vec3(3.f, 5.f, -4.f);
    cloudRight->transform.scale = glm::vec3(1.5f, 0.4f, 1.0f);
    cloudRight->tint = TINT_CLOUD;
    cloudRight->texID = rockTexID;

    GameObject* sun = CreateGameObject(sunMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    sun->transform.position = glm::vec3(0.f, 50.f, 0.f);
    sun->transform.scale = glm::vec3(8.f);
    sun->enableLighting = false;
    sun->texID = 0;
    SunGO = sun;

    GameObject* moon = CreateGameObject(moonMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    moon->transform.position = glm::vec3(0.f, -50.f, 0.f);
    moon->transform.scale = glm::vec3(2.f);
    moon->enableLighting = false;
    moon->texID = 0;
    MoonGO = moon;

    srand((unsigned int)time(nullptr));

    spawner.AddModel(hayBaleMesh, colormapTexID);
    spawner.AddModel(pineFallMesh, colormapTexID);
    spawner.AddModel(trunkMesh, colormapTexID);
    spawner.AddModel(pineMesh, colormapTexID);

    spawner.AddSpawnPoint(glm::vec3(-5.f, 0.f, -3.f));
    spawner.AddSpawnPoint(glm::vec3(5.f, 0.f, -3.f));
    spawner.AddSpawnPoint(glm::vec3(-4.f, 0.f, 4.f));
    spawner.AddSpawnPoint(glm::vec3(4.f, 0.f, 4.f));
    spawner.AddSpawnPoint(glm::vec3(0.f, 0.f, 6.f));
    spawner.AddSpawnPoint(glm::vec3(2.f, 0.f, -6.f));

    spawner.SpawnAll(this, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl", 3);

    renderer.SetClearColor(0.f, 0.f, 0.f, 1.f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (size_t i = 0; i < compiledPrograms.size(); i++) {
        glUseProgram(compiledPrograms[i]);
        glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

        glUniform3f(glGetUniformLocation(compiledPrograms[i], "lightColor"),     LigthColor.x,     LigthColor.y,     LigthColor.z);
        glUniform1f(glGetUniformLocation(compiledPrograms[i], "ambientStrength"), AmbientStrength);
    }
}

void Game::Update()
{
    flashlight.HandleInput();

    glm::vec3 ambientColor = glm::vec3(0.0f);

    glm::vec3 sunPosition;
    glm::vec3 sunDirection;
    glm::vec3 sunColor = glm::vec3(1.0f, 1.0f, 0.0f);

    glm::vec3 moonDirection;
    glm::vec3 moonPosition;
    glm::vec3 moonColor = glm::vec3(0.5f, 0.5f, 1.0f);

    float currentAmbientStrength = AmbientStrength;

    float cycleDuration = 20.0f;
    float normalizedTime = TM->GetGlobalTime() / cycleDuration;

    float radius = 20.0f;

    glm::vec3 WorldCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    float pi = glm::pi<float>();
    float theta = normalizedTime * 2.0f * pi;

    // Rotación 2D del punto (radius, radius) alrededor de WorldCenter; la luna va desfasada pi radianes
    radius = 1.41f * radius;
    sunPosition.x = WorldCenter.x + radius * cos(theta) - radius * sin(theta);
    sunPosition.y = WorldCenter.y + radius * sin(theta) + radius * cos(theta);
    sunPosition.z = WorldCenter.z - radius * cos(theta);

    moonPosition.x = WorldCenter.x + radius * cos(theta - pi) - radius * sin(theta - pi);
    moonPosition.y = WorldCenter.y + radius * sin(theta - pi) + radius * cos(theta - pi);
    moonPosition.z = WorldCenter.z - radius * cos(theta - pi);

    SunGO->transform.position = sunPosition;
    MoonGO->transform.position = moonPosition;

    sunDirection = glm::normalize(sunPosition - WorldCenter);
    moonDirection = glm::normalize(moonPosition - WorldCenter);

    currentAmbientStrength = AmbientStrength * (sunPosition.y > 0.0f ? 1.0f : 0.1f);

    if (sunPosition.y > 0.0f) {
        sunColor = glm::vec3(1.0f, 0.95f, 0.85f);
    }

    if(moonPosition.y > 0.0f) {
        moonColor = glm::vec3(0.5f, 0.5f, 1.0f);
    }

    // El índice se invierte para que el cielo se vuelva azul cuando sale la luna
    int ColorIndex = static_cast<int>(normalizedTime * ambientColors.size()) % ambientColors.size();
    int InvertIndex = ambientColors.size() - 1 - ColorIndex;
    ambientColor = ambientColors[InvertIndex];

    renderer.SetClearColor(ambientColor.r, ambientColor.g, ambientColor.b, 1.0f);

    for (size_t i = 0; i < compiledPrograms.size(); i++)
    {
        camera.Update(&compiledPrograms[i], WINDOW_WIDTH, WINDOW_HEIGHT);
        flashlight.Update(&compiledPrograms[i], camera.transform.position, camera.GetForwardVector());

        glUseProgram(compiledPrograms[i]);

        glUniform3f(glGetUniformLocation(compiledPrograms[i], "uSunDirection"), sunDirection.x, sunDirection.y, sunDirection.z);
        glUniform3f(glGetUniformLocation(compiledPrograms[i], "uSunColor"), sunColor.x, sunColor.y, sunColor.z);

        glUniform3f(glGetUniformLocation(compiledPrograms[i], "uMoonDirection"), moonDirection.x, moonDirection.y, moonDirection.z);
        glUniform3f(glGetUniformLocation(compiledPrograms[i], "uMoonColor"), moonColor.x, moonColor.y, moonColor.z);

        glUniform3f(glGetUniformLocation(compiledPrograms[i], "lightColor"), ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform1f(glGetUniformLocation(compiledPrograms[i], "ambientStrength"), currentAmbientStrength);
    }
}

void Game::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (GameObject* obj : sceneObjects)
    {
        renderer.RenderObject(*obj, *obj->mesh, obj->shaderProgram->programId,
                              obj->mesh->GetVertexCount(), obj->texID, obj->tint);
    }

    glFlush();
    glfwSwapBuffers(windowManager->window);
}

void Game::Stop()
{
    glDeleteTextures(1, &trollTexID);
    glDeleteTextures(1, &rockTexID);
    glDeleteTextures(1, &colormapTexID);

    for (GameObject* obj : sceneObjects)
        delete obj;

    sceneObjects.clear();
    glfwTerminate();
}

GameObject* Game::CreateGameObject(Mesh* mesh, const char* vertexShader, const char* geometryShader, const char* fragmentShader)
{
    ShaderProgram* shader = new ShaderProgram(vertexShader, geometryShader, fragmentShader);
    compiledPrograms.push_back(shader->programId);

    GameObject* obj = new GameObject(mesh, shader);
    sceneObjects.push_back(obj);

    return obj;
}
