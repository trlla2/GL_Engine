#include "Game.h"
#include "ShaderProgram.h"
#include "InputManager.h"
#include "TimeManager.h"
#include <gtc/constants.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

const glm::vec4 TINT_TROLL_GREEN = glm::vec4(0.4f, 1.0f, 0.4f, 1.0f);
const glm::vec4 TINT_TROLL_BLUE  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 TINT_TROLL_TEAL  = glm::vec4(0.4f, 0.9f, 0.8f, 1.0f);
const glm::vec4 TINT_ROCK_NORMAL = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 TINT_CLOUD       = glm::vec4(1.2f, 1.2f, 1.2f, 1.0f);
const glm::vec4 TINT_GRASS       = glm::vec4(0.36f, 0.62f, 0.38f, 1.0f);

const float CYCLE_DURATION = 90.0f;

const glm::vec3 SUN_HORIZON_COLOR = glm::vec3(1.0f, 0.55f, 0.30f);
const glm::vec3 SUN_NOON_COLOR    = glm::vec3(1.0f, 0.95f, 0.85f);
const glm::vec3 MOON_COLOR        = glm::vec3(0.45f, 0.55f, 0.90f);
const float SUN_INTENSITY  = 1.0f;
const float MOON_INTENSITY = 0.55f;

// Color del cielo y de la luz ambiental según la altura del sol (-1 = medianoche, 1 = mediodía)
struct SkyKey
{
    float sunHeight;
    glm::vec3 sky;
    glm::vec3 ambient;
};

const SkyKey SKY_KEYS[] =
{
    { -1.00f, glm::vec3(0.01f, 0.02f, 0.07f), glm::vec3(0.08f, 0.10f, 0.20f) },
    { -0.40f, glm::vec3(0.03f, 0.05f, 0.15f), glm::vec3(0.09f, 0.11f, 0.21f) },
    { -0.20f, glm::vec3(0.20f, 0.12f, 0.32f), glm::vec3(0.12f, 0.10f, 0.20f) },
    { -0.08f, glm::vec3(0.62f, 0.32f, 0.45f), glm::vec3(0.20f, 0.13f, 0.20f) },
    {  0.02f, glm::vec3(0.95f, 0.58f, 0.42f), glm::vec3(0.30f, 0.20f, 0.18f) },
    {  0.12f, glm::vec3(0.95f, 0.80f, 0.55f), glm::vec3(0.33f, 0.27f, 0.22f) },
    {  0.25f, glm::vec3(0.62f, 0.82f, 0.88f), glm::vec3(0.30f, 0.31f, 0.32f) },
    {  0.50f, glm::vec3(0.50f, 0.76f, 0.95f), glm::vec3(0.30f, 0.33f, 0.38f) },
    {  1.00f, glm::vec3(0.33f, 0.60f, 0.92f), glm::vec3(0.30f, 0.34f, 0.40f) },
};

// Interpola entre claves con smoothstep para que no haya saltos de color
void SampleSky(float sunHeight, glm::vec3& sky, glm::vec3& ambient)
{
    const int count = sizeof(SKY_KEYS) / sizeof(SKY_KEYS[0]);

    sky = SKY_KEYS[count - 1].sky;
    ambient = SKY_KEYS[count - 1].ambient;

    if (sunHeight <= SKY_KEYS[0].sunHeight)
    {
        sky = SKY_KEYS[0].sky;
        ambient = SKY_KEYS[0].ambient;
        return;
    }

    for (int i = 0; i < count - 1; i++)
    {
        const SkyKey& a = SKY_KEYS[i];
        const SkyKey& b = SKY_KEYS[i + 1];
        if (sunHeight <= b.sunHeight)
        {
            float t = glm::smoothstep(a.sunHeight, b.sunHeight, sunHeight);
            sky = glm::mix(a.sky, b.sky, t);
            ambient = glm::mix(a.ambient, b.ambient, t);
            return;
        }
    }
}

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
    Mesh* groundMesh = objLoader.LoadOBJMesh("Assets/Modelos/TerrainAssets/block-grass-low-large.obj");
    meshes.push_back(groundMesh);

    // El sol y la luna usan la misma malla: el loader solo admite caras trianguladas
    Mesh* sunMesh = objLoader.LoadOBJMesh("Assets/Modelos/Sun.obj");
    meshes.push_back(sunMesh);
    Mesh* moonMesh = objLoader.LoadOBJMesh("Assets/Modelos/Sun.obj");
    meshes.push_back(moonMesh);

    GameObject* ground = CreateGameObject(groundMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
    ground->transform.position = glm::vec3(0.f, -0.5f, 0.f);
    ground->transform.scale = glm::vec3(25.f, 1.f, 25.f);
    ground->tint = TINT_GRASS;
    ground->texID = 0;

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
    moon->tint = glm::vec4(0.85f, 0.90f, 1.0f, 1.0f);
    MoonGO = moon;

    srand((unsigned int)time(nullptr));

    spawner.AddModel(hayBaleMesh, colormapTexID);
    spawner.AddModel(pineFallMesh, colormapTexID);
    spawner.AddModel(trunkMesh, colormapTexID);
    spawner.AddModel(pineMesh, colormapTexID);

    spawner.AddSpawnPoint(glm::vec3(-5.5f, 0.f, -2.f));
    spawner.AddSpawnPoint(glm::vec3(5.5f, 0.f, -2.f));
    spawner.AddSpawnPoint(glm::vec3(-6.5f, 0.f, 2.f));
    spawner.AddSpawnPoint(glm::vec3(6.5f, 0.f, 2.f));
    spawner.AddSpawnPoint(glm::vec3(-2.f, 0.f, -6.f));
    spawner.AddSpawnPoint(glm::vec3(2.5f, 0.f, -7.f));

    spawner.SpawnAll(this, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl", 3);

    renderer.SetClearColor(0.f, 0.f, 0.f, 1.f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (size_t i = 0; i < compiledPrograms.size(); i++) {
        glUseProgram(compiledPrograms[i]);
        glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
        glUniform1f(glGetUniformLocation(compiledPrograms[i], "ambientStrength"), 1.0f);
    }
}

void Game::Update()
{
    flashlight.HandleInput();

    float pi = glm::pi<float>();
    float theta = (TM->GetGlobalTime() / CYCLE_DURATION) * 2.0f * pi;

    // Rotación 2D del punto (radius, radius) alrededor de WorldCenter; la luna va desfasada pi radianes
    glm::vec3 WorldCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    float radius = 1.41f * 20.0f;

    glm::vec3 sunPosition;
    sunPosition.x = WorldCenter.x + radius * cos(theta) - radius * sin(theta);
    sunPosition.y = WorldCenter.y + radius * sin(theta) + radius * cos(theta);
    sunPosition.z = WorldCenter.z + radius * cos(theta);

    glm::vec3 moonPosition;
    moonPosition.x = WorldCenter.x + radius * cos(theta - pi) - radius * sin(theta - pi);
    moonPosition.y = WorldCenter.y + radius * sin(theta - pi) + radius * cos(theta - pi);
    moonPosition.z = WorldCenter.z + radius * cos(theta - pi);

    SunGO->transform.position = sunPosition;
    MoonGO->transform.position = moonPosition;

    glm::vec3 sunDirection = glm::normalize(sunPosition - WorldCenter);
    glm::vec3 moonDirection = glm::normalize(moonPosition - WorldCenter);

    glm::vec3 skyColor;
    glm::vec3 ambientColor;
    SampleSky(sunDirection.y, skyColor, ambientColor);

    // El sol se vuelve anaranjado cerca del horizonte y ambas luces se apagan gradualmente al ponerse
    glm::vec3 sunTint = glm::mix(SUN_HORIZON_COLOR, SUN_NOON_COLOR, glm::smoothstep(0.0f, 0.4f, sunDirection.y));
    glm::vec3 sunColor = sunTint * SUN_INTENSITY * glm::smoothstep(-0.08f, 0.15f, sunDirection.y);
    glm::vec3 moonColor = MOON_COLOR * MOON_INTENSITY * glm::smoothstep(-0.08f, 0.15f, moonDirection.y);

    SunGO->tint = glm::vec4(sunTint, 1.0f);

    renderer.SetClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);

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
