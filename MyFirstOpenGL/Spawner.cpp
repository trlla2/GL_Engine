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

    // lista de indices de todos los spawnpoints
    std::vector<int> indices;
    for (int i = 0; i < (int)spawnPoints.size(); i++) indices.push_back(i);

    // mezclar los indices (fisher-yates) para coger unos pocos al azar sin repetir
    for (int i = (int)indices.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i]; indices[i] = indices[j]; indices[j] = tmp;
    }

    // no spawnear mas puntos de los que hay
    if (count > (int)indices.size()) count = (int)indices.size();

    for (int k = 0; k < count; k++) {
        const glm::vec3& point = spawnPoints[indices[k]];

        // elegir modelo aleatorio de la lista
        int index = rand() % (int)models.size();
        const SpawnModel& model = models[index];

        GameObject* obj = game->CreateGameObject(model.mesh, vertexShader, geometryShader, fragmentShader);
        obj->transform.position = point;
        obj->texID = model.texID;
        obj->tint = model.tint;

        // giro aleatorio solo en Y para que el modelo quede de pie
        // el renderer usa rotation como eje-angulo: direccion = eje, longitud = grados
        float angle = RandomRange(0.f, 360.f);
        obj->transform.rotation = glm::vec3(0.f, angle, 0.f);

        // escala uniforme para no deformar el modelo
        float s = RandomRange(0.6f, 1.5f);
        obj->transform.scale = glm::vec3(s);
    }
}
