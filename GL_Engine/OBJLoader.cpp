#include "OBJLoader.h"
#include "FileLoader.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <glm.hpp>

// Solo admite caras trianguladas con formato v/vt/vn
Mesh* OBJLoader::LoadOBJMesh(const std::string& filePath) {

	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	std::string line;
	std::stringstream ss;
	std::string prefix;
	glm::vec3 tmpVec3;
	glm::vec2 tmpVec2;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uvs;

	// Datos indexados tal como aparecen en el archivo
	std::vector<float> tmpVertices;
	std::vector<float> tmpNormals;
	std::vector<float> tmpUVs;

	while (std::getline(file, line)) {

		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "v") {
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			tmpVertices.push_back(tmpVec3.x);
			tmpVertices.push_back(tmpVec3.y);
			tmpVertices.push_back(tmpVec3.z);
		}

		else if (prefix == "vt") {
			ss >> tmpVec2.x >> tmpVec2.y;

			tmpUVs.push_back(tmpVec2.x);
			tmpUVs.push_back(tmpVec2.y);
		}

		else if (prefix == "vn") {
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			tmpNormals.push_back(tmpVec3.x);
			tmpNormals.push_back(tmpVec3.y);
			tmpNormals.push_back(tmpVec3.z);
		}

		else if (prefix == "f") {

			int vertexData;
			short counter = 0;

			// Cada vértice de la cara sigue el patrón v/vt/vn con índices base 1
			while (ss >> vertexData) {

				switch (counter) {
				case 0:
					vertices.push_back(tmpVertices[(vertexData - 1) * 3]);
					vertices.push_back(tmpVertices[((vertexData - 1) * 3) + 1]);
					vertices.push_back(tmpVertices[((vertexData - 1) * 3) + 2]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 1:
					uvs.push_back(tmpUVs[(vertexData - 1) * 2]);
					uvs.push_back(tmpUVs[((vertexData - 1) * 2) + 1]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 2:
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
