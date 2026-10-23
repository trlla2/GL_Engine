#pragma once
#include <string>
#include "Mesh.h"

class OBJLoader
{
public:
    Mesh* LoadOBJMesh(const std::string& filePath);
};
