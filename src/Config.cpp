#include "Config.hpp"
#include <fstream>

std::map<std::string, int> config;

bool init_config(std::string path)
{
	std::fstream fin(path);
	if (!fin) return false;
	std::string param;
	int value;
	while (fin >> param >> value) config[param] = value;
	fin.close();
    return true;
}
