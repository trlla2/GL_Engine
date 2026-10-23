#include "Spawner.h"
#include "Game.h"
#include "GameObjects.h"
#include <cstdlib>

void Spawner::AddModel(Mesh* mesh, GLuint texID, glm::vec4 tint) {
    models.push_back({ mesh, texID, tint });
}

void Spawner::AddSpawnPoint(const glm::vec3& point) {
    spawnPoints.push_back(point);
}

float Spawner::RandomRange(float min, float max) {
    float t = (float)rand() / (float)RAND_MAX;
    return min + t * (max - min);
}

void Spawner::SpawnAll(Game* game, const char* vertexShader, const char* geometryShader, const char* fragmentShader, int count) {
    if (models.empty() || spawnPoints.empty()) return;

    std::vector<int> indices;
    for (int i = 0; i < (int)spawnPoints.size(); i++) indices.push_back(i);

    // Fisher-Yates: elige spawn points al azar sin repetir
    for (int i = (int)indices.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i]; indices[i] = indices[j]; indices[j] = tmp;
    }

    if (count > (int)indices.size()) count = (int)indices.size();

    for (int k = 0; k < count; k++) {
        const glm::vec3& point = spawnPoints[indices[k]];

        int index = rand() % (int)models.size();
        const SpawnModel& model = models[index];

        GameObject* obj = game->CreateGameObject(model.mesh, vertexShader, geometryShader, fragmentShader);
        obj->transform.position = point;
        obj->texID = model.texID;
        obj->tint = model.tint;

        // Rotación solo en Y para que el modelo quede de pie
        float angle = RandomRange(0.f, 360.f);
        obj->transform.rotation = glm::vec3(0.f, angle, 0.f);

        float s = RandomRange(0.6f, 1.5f);
        obj->transform.scale = glm::vec3(s);
    }
}
