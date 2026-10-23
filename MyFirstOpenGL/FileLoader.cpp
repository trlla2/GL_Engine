#include "FileLoader.h"
#include <iostream>
#include <fstream>
#include <string>

std::string FileLoader::Load_File(const std::string& filePath)
{
	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	file.close();

	return fileContent;
}
