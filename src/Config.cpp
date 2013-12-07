#include "Config.hpp"
#include <fstream>

std::map<std::string, int> config;

namespace conf
{
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
	
	bool pressing(Key key, sf::Keyboard::Key code)
	{
		if (key == UP) return code == sf::Keyboard::W || code == sf::Keyboard::Up;
		if (key == DOWN) return code == sf::Keyboard::S || code == sf::Keyboard::Down;
		if (key == LEFT) return code == sf::Keyboard::A || code == sf::Keyboard::Left;
		if (key == RIGHT) return code == sf::Keyboard::D || code == sf::Keyboard::Right;
		return false;
	}

	bool pressing(Key key)
	{
		if (key == UP) return sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		if (key == DOWN) return sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		if (key == LEFT) return sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		if (key == RIGHT) return sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		return false;
	}
}