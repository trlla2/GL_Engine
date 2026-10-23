#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include "Mesh.h"

class Game;

struct SpawnModel {
    Mesh* mesh;
    GLuint texID;
    glm::vec4 tint;
};

class Spawner {
public:
    void AddModel(Mesh* mesh, GLuint texID, glm::vec4 tint = glm::vec4(1.f));
    void AddSpawnPoint(const glm::vec3& point);

    void SpawnAll(Game* game, const char* vertexShader, const char* geometryShader, const char* fragmentShader, int count = 3);

private:
    std::vector<SpawnModel> models;
    std::vector<glm::vec3> spawnPoints;

    float RandomRange(float min, float max);
};
