#pragma once
#include <fstream>

class FileLoader
{
public:
	//Funcion que devolvera una string con todo el archivo leido
	std::string Load_File(const std::string& filePath);
};

std::string FileLoader::Load_File(const std::string& filePath) 
{

	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	//Lanzamos error si el archivo no se ha podido abrir
	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Leemos el contenido y lo volcamos a la variable auxiliar
	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	//Cerramos stream de datos y devolvemos contenido
	file.close();

	return fileContent;
}