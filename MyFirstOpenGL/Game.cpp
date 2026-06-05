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
    // textura compartida por los modelos del spawner
    colormapTexID = textureLoader.Load("Assets/Texturas/colormap.png");

    // Cargar mallas (reutilizar en todas sus instancias)
    Mesh* trollMesh = objLoader.LoadOBJMesh("Assets/Modelos/troll.obj");
    meshes.push_back(trollMesh);
    Mesh* rockMesh = objLoader.LoadOBJMesh("Assets/Modelos/rock.obj");
    meshes.push_back(rockMesh);

    // mallas para el spawner (todas usan colormap.png)
    Mesh* hayBaleMesh = objLoader.LoadOBJMesh("Assets/Modelos/hay-bale.obj");
    meshes.push_back(hayBaleMesh);
    Mesh* pineFallMesh = objLoader.LoadOBJMesh("Assets/Modelos/pine-fall.obj");
    meshes.push_back(pineFallMesh);
    Mesh* trunkMesh = objLoader.LoadOBJMesh("Assets/Modelos/trunk.obj");
    meshes.push_back(trunkMesh);
    Mesh* pineMesh = objLoader.LoadOBJMesh("Assets/Modelos/pine.obj");
    meshes.push_back(pineMesh);

    // Cargar mallas para el Sol y la Luna
    //El mismo modelo pq LOS MODELOS D INTERNER USAN QUADS I NO LO PODEMOS LEER
    //Sun.obj ha sido 100% generado por mi chatty gpt para q sea copatible con el formato
    Mesh* sunMesh = objLoader.LoadOBJMesh("Assets/Modelos/Sun.obj");
    meshes.push_back(sunMesh);
    Mesh* moonMesh = objLoader.LoadOBJMesh("Assets/Modelos/Sun.obj");
    meshes.push_back(moonMesh);

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

	GameObject* sun = CreateGameObject(sunMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
	sun->transform.position = glm::vec3(0.f, 50.f, 0.f);
	sun->transform.scale = glm::vec3(8.f);
	sun->enableLighting = false; // El Sol no recibe iluminaci�n
	sun->texID = 0; // El Sol no tiene textura, solo color
	SunGO = sun; // Guardar referencia

	GameObject* moon = CreateGameObject(moonMesh, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl");
	moon->transform.position = glm::vec3(0.f, -50.f, 0.f);
	moon->transform.scale = glm::vec3(2.f);
	moon->enableLighting = false; // La Luna no recibe iluminaci�n
	moon->texID = 0; // La Luna no tiene textura, solo color
	MoonGO = moon; // Guardar referencia

    // semilla aleatoria para que el spawn cambie en cada ejecucion
    srand((unsigned int)time(nullptr));

    // configurar modelos disponibles para el spawner (todos usan colormap.png)
    spawner.AddModel(hayBaleMesh, colormapTexID);
    spawner.AddModel(pineFallMesh, colormapTexID);
    spawner.AddModel(trunkMesh, colormapTexID);
    spawner.AddModel(pineMesh, colormapTexID);

    // puntos donde puede aparecer un modelo (a ras de suelo)
    spawner.AddSpawnPoint(glm::vec3(-5.f, 0.f, -3.f));
    spawner.AddSpawnPoint(glm::vec3(5.f, 0.f, -3.f));
    spawner.AddSpawnPoint(glm::vec3(-4.f, 0.f, 4.f));
    spawner.AddSpawnPoint(glm::vec3(4.f, 0.f, 4.f));
    spawner.AddSpawnPoint(glm::vec3(0.f, 0.f, 6.f));
    spawner.AddSpawnPoint(glm::vec3(2.f, 0.f, -6.f));

    // escoger 3 spawnpoints al azar y crear un modelo aleatorio en cada uno
    spawner.SpawnAll(this, "CubeVertexShader.glsl", "CubeGeometryShader.glsl", "CubeFragmentShader.glsl", 3);

    //Definimos color para limpiar el buffer de color
    renderer.SetClearColor(0.f, 0.f, 0.f, 1.f);

    //Definimos modo de dibujo para cada cara
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Asignar valores iniciales al shader
    for (size_t i = 0; i < compiledPrograms.size(); i++) {
        glUseProgram(compiledPrograms[i]);
        glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

        // Ambient light
        glUniform3f(glGetUniformLocation(compiledPrograms[i], "lightColor"),     LigthColor.x,     LigthColor.y,     LigthColor.z);
        glUniform1f(glGetUniformLocation(compiledPrograms[i], "ambientStrength"), AmbientStrength);
    }
}

void Game::Update()
{

    //  una vez por frame
    flashlight.HandleInput();

	//IA Prompt: Implementa un sistema de d�a y noche que haga orbitar al sol y la luna alrededor del punto 0,0,0 , cambiando sus posiciones, colores e intensidades de luz, y el color ambiental para simular el paso del tiempo. El ciclo completo debe durar 20 segundos.
    //C�lculos de la mec�nica orbital (Duraci�n del ciclo = 20 segundos)
    float time = TM->GetGlobalTime();
    float cycleDuration = 20.0f;
	float angle = (time / cycleDuration) * 2.0f * glm::pi<float>(); // angulo = delta * 2PI

    //Radio de la �rbita alrededor de (0,0,0)
    float orbitRadius = 50.0f;

    // Posici�n del Sol y la Luna (�rbita vertical en el plano Y-Z)
    glm::vec3 sunPos = glm::vec3(0.0f, orbitRadius * sin(angle), orbitRadius * cos(angle));
    //Luna siempre a 180� del Sol (+ PI)
    glm::vec3 moonPos = glm::vec3(0.0f, orbitRadius * sin(angle + glm::pi<float>()), orbitRadius * cos(angle + glm::pi<float>()));

    // Actualizar posiciones de los GameObjects orbitales
    if (SunGO != nullptr) {
        SunGO->transform.position = sunPos;
    }
    if (MoonGO != nullptr) {
        MoonGO->transform.position = moonPos;
    }

    //2.C�lculo de Direcciones e Intensidades de Luz
    glm::vec3 sunDirection = glm::normalize(sunPos - glm::vec3(0,0,0));
    glm::vec3 moonDirection = glm::normalize(moonPos - glm::vec3(0,0,0));

    //Si est�n bajo del suelo (Y <= 0), dejan de emitir luz
    glm::vec3 sunColor = glm::vec3(0.0f);
    if (sunPos.y > 0.0f) {
        sunColor = glm::vec3(1.0f, 0.95f, 0.85f); // Sol: Luz fuerte y c�lida
    }

    glm::vec3 moonColor = glm::vec3(0.0f);
    if (moonPos.y > 0.0f) {
        moonColor = glm::vec3(0.15f, 0.22f, 0.35f); // Luna: Luz tenue y fr�a
    }

    // 3. Interpolaci�n de ambient ligth
    // t va de 0.0 (medianoche) a 1.0 (mediod�a) basado en el seno del �ngulo orbital del sol
    float t = (sin(angle) + 1.0f) * 0.5f;
    glm::vec3 midnightAmbient = glm::vec3(0.02f, 0.02f, 0.09f); // Azul oscuro
    glm::vec3 middayAmbient = glm::vec3(0.22f, 0.20f, 0.12f);    // Color amarillentos
    glm::vec3 ambientColor = glm::mix(midnightAmbient, middayAmbient, t);
    float currentAmbientStrength = glm::mix(0.05f, AmbientStrength, t);

    // Actualizar el color de fondo para fundirse de d�a a noche
    renderer.SetClearColor(ambientColor.r, ambientColor.g, ambientColor.b, 1.0f);



    // actualizar las matrizes de la camara, ambient,  y flaslight
    for (size_t i = 0; i < compiledPrograms.size(); i++)
    {
        camera.Update(&compiledPrograms[i], WINDOW_WIDTH, WINDOW_HEIGHT);
        flashlight.Update(&compiledPrograms[i], camera.transform.position, camera.GetForwardVector());

        glUseProgram(compiledPrograms[i]);

        // Enviar par�metros direccionales y ambientales
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
    glDeleteTextures(1, &colormapTexID);

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