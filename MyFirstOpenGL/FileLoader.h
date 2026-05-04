#pragma once
#include <fstream>

class FileLoader
{
public:
	//Funcion que devolvera una string con todo el archivo leido
	std::string Load_File(const std::string& filePath);
};