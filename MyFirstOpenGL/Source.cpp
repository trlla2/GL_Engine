#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stb_image.h>

#include "InputManager.h"
#include "TimeManager.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Mesh.h"
#include "GameObjects.h"
#include "Camera.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

std::vector<GLuint> compiledPrograms;
std::vector<Mesh> meshes;

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
	//Definir nuevo tamano del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
	
	// Actualizar uniform para todos los programas
	for (size_t i = 0; i < compiledPrograms.size(); i++) {
		glUseProgram(compiledPrograms[i]);
		glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
	}
}

GLuint loadTexture(const char* path)
{
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true); // OpenGL tiene la Y invertida
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

	if (data)
	{
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Error: no se pudo cargar la textura: " << path << std::endl;
	}

	stbi_image_free(data);
	return texID;
}
#pragma region IA Prompt
//Propt a la IA:
/*
* //Funcion que leera un .obj y devolvera un modelo para poder ser renderizado
Model LoadOBJModel(const std::string& filePath) {

	//Verifico archivo y si no puedo abrirlo cierro aplicativo
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Variables lectura fichero
	std::string line;
	std::stringstream ss;
	std::string prefix;
	glm::vec3 tmpVec3;
	glm::vec2 tmpVec2;

	//Variables elemento modelo
	std::vector<float> vertexs;
	std::vector<float> vertexNormal;
	std::vector<float> textureCoordinates;

	//Variables temporales para algoritmos de sort
	std::vector<float> tmpVertexs;
	std::vector<float> tmpNormals;
	std::vector<float> tmpTextureCoordinates;

	//Recorremos archivo linea por linea
	while (std::getline(file, line)) {

		//Por cada linea reviso el prefijo del archivo que me indica que estoy analizando
		ss.clear();
		ss.str(line);
		ss >> prefix;

		//Estoy leyendo un vertice
		if (prefix == "v") {

			//Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			//Almaceno en mi vector de vertices los valores
			tmpVertexs.push_back(tmpVec3.x);
			tmpVertexs.push_back(tmpVec3.y);
			tmpVertexs.push_back(tmpVec3.z);
		}

		//Estoy leyendo una UV (texture coordinate)
		else if (prefix == "vt") {

			//Las UVs son siempre imagenes 2D asi que uso el tmpvec2 para almacenarlas
			ss >> tmpVec2.x >> tmpVec2.y;

			//Almaceno en mi vector temporal las UVs
			tmpTextureCoordinates.push_back(tmpVec2.x);
			tmpTextureCoordinates.push_back(tmpVec2.y);

		}

		//Estoy leyendo una normal
		else if (prefix == "vn") {

			//Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			//Almaceno en mi vector temporal de normales las normales
			tmpNormals.push_back(tmpVec3.x);
			tmpNormals.push_back(tmpVec3.y);
			tmpNormals.push_back(tmpVec3.z);

		}

		//Estoy leyendo una cara
		else if (prefix == "f") {

			int vertexData;
			short counter = 0;

			//Obtengo todos los valores hasta un espacio
			while (ss >> vertexData) {

				//En orden cada numero sigue el patron de vertice/uv/normal
				switch (counter) {
				case 0:
					//Si es un vertice lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec3, salto 1 / y aumento el contador en 1
					vertexs.push_back(tmpVertexs[(vertexData - 1) * 3]);
					vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 1]);
					vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 2]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 1:
					//Si es un uv lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec2, salto 1 / y aumento el contador en 1
					textureCoordinates.push_back(tmpTextureCoordinates[(vertexData - 1) * 2]);
					textureCoordinates.push_back(tmpTextureCoordinates[((vertexData - 1) * 2) + 1]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 2:
					//Si es una normal la almaceno - 1 por el offset y almaceno tres seguidos al ser un vec3, salto 1 / y reinicio
					vertexNormal.push_back(tmpNormals[(vertexData - 1) * 3]);
					vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 1]);
					vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 2]);
					counter = 0;
					break;
				}
			}
		}
	}
	return Model(vertexs, textureCoordinates, vertexNormal);
}

rerwite this functiion to work with my Mesh Class
(esta función la he sacado de el codigo de classe)
*/

#pragma endregion
//Funcion que leera un .obj y devolvera un mesh para poder ser renderizado
Mesh* LoadOBJMesh(const std::string& filePath) {

	//Verifico archivo y si no puedo abrirlo cierro aplicativo
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Variables lectura fichero
	std::string line;
	std::stringstream ss;
	std::string prefix;
	glm::vec3 tmpVec3;
	glm::vec2 tmpVec2;

	//Variables elemento modelo
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uvs;

	//Variables temporales para algoritmos de sort
	std::vector<float> tmpVertices;
	std::vector<float> tmpNormals;
	std::vector<float> tmpUVs;

	//Recorremos archivo linea por linea
	while (std::getline(file, line)) {

		//Por cada linea reviso el prefijo del archivo que me indica que estoy analizando
		ss.clear();
		ss.str(line);
		ss >> prefix;

		//Estoy leyendo un vertice
		if (prefix == "v") {

			//Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			//Almaceno en mi vector de vertices los valores
			tmpVertices.push_back(tmpVec3.x);
			tmpVertices.push_back(tmpVec3.y);
			tmpVertices.push_back(tmpVec3.z);
		}

		//Estoy leyendo una UV (texture coordinate)
		else if (prefix == "vt") {

			//Las UVs son siempre imagenes 2D asi que uso el tmpvec2 para almacenarlas
			ss >> tmpVec2.x >> tmpVec2.y;

			//Almaceno en mi vector temporal las UVs
			tmpUVs.push_back(tmpVec2.x);
			tmpUVs.push_back(tmpVec2.y);

		}

		//Estoy leyendo una normal
		else if (prefix == "vn") {

			//Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			//Almaceno en mi vector temporal de normales las normales
			tmpNormals.push_back(tmpVec3.x);
			tmpNormals.push_back(tmpVec3.y);
			tmpNormals.push_back(tmpVec3.z);

		}

		//Estoy leyendo una cara
		else if (prefix == "f") {

			int vertexData;
			short counter = 0;

			//Obtengo todos los valores hasta un espacio
			while (ss >> vertexData) {

				//En orden cada numero sigue el patron de vertice/uv/normal
				switch (counter) {
				case 0:
					//Si es un vertice lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec3, salto 1 / y aumento el contador en 1
					vertices.push_back(tmpVertices[(vertexData - 1) * 3]);
					vertices.push_back(tmpVertices[((vertexData - 1) * 3) + 1]);
					vertices.push_back(tmpVertices[((vertexData - 1) * 3) + 2]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 1:
					//Si es un uv lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec2, salto 1 / y aumento el contador en 1
					uvs.push_back(tmpUVs[(vertexData - 1) * 2]);
					uvs.push_back(tmpUVs[((vertexData - 1) * 2) + 1]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 2:
					//Si es una normal la almaceno - 1 por el offset y almaceno tres seguidos al ser un vec3, salto 1 / y reinicio
					normals.push_back(tmpNormals[(vertexData - 1) * 3]);
					normals.push_back(tmpNormals[((vertexData - 1) * 3) + 1]);
					normals.push_back(tmpNormals[((vertexData - 1) * 3) + 2]);
					counter = 0;
					break;
				}
			}
		}
	}

	file.close();

	return new Mesh(vertices, uvs, normals);
}

void main() 
{
	// Inicializar el gestor de tiempo
	TM->Init();

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos función de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	glEnable(GL_CULL_FACE);
	//Activamos depth test
	glEnable(GL_DEPTH_TEST);

	//Indicamos lado del culling
	glCullFace(GL_BACK);

	if (glewInit() == GLEW_OK)
	{
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
		Mesh* trollMesh = LoadOBJMesh("Assets/Modelos/troll.obj");
		meshes.push_back(*trollMesh);

		// Crear el Troll GameObject
		Troll* troll = new Troll(trollMesh, trollShader);


		// Crear renderer
		Renderer renderer;

		//Definimos color para limpiar el buffer de color
		renderer.SetClearColor(0.f, 0.f, 0.f, 1.f);

		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Asignar valores iniciales al shader
		for (size_t i = 0; i < compiledPrograms.size(); i++) {
			glUseProgram(compiledPrograms[i]);
			glUniform2f(glGetUniformLocation(compiledPrograms[i], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
		}

		//Generamos el game loop

		// Input variables
		bool showWireframe = false;

		// Activamos el Input Manager con la ventana 
		IM->Init(window);

		//Generamos el game loop
		while (!glfwWindowShouldClose(window))
		{
			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			// Updateamos el Input Manager
			IM->Listen();

			// Updateamos el Time Manager
			TM->Update();



			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			
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

			// Renderizar mesh
			renderer.RenderObject(*troll, *troll->mesh, troll->shaderProgram->programId, troll->mesh->GetVertexCount(), loadTexture("Assets/Texturas/troll.png"), glm::vec4(0.f, 0.f, 0.f, 0.f));
			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

		// Cleanup
		delete troll;

	}
	else
	{
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();
}