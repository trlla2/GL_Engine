#pragma once
#include <string>
#include "Mesh.h"

class OBJLoader
{
public:
    // Lee un archivo .obj
    Mesh* LoadOBJMesh(const std::string& filePath);
};