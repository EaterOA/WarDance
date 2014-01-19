#include "Config.hpp"
#include <fstream>

std::map<std::string, int> config;

namespace conf
{
	bool init_config(std::string path)
	{
		std::fstream fin(path.c_str());
		if (!fin) return false;
		std::string param;
		int value;
		while (fin >> param >> value) config[param] = value;
		fin.close();

		config["level"] = 1;
		config["num_levels"] = 5;
		config["highscore"] = 0;
		return true;
	}

	bool clicking(Button button)
	{
		if (button == B_LEFT) return sf::Mouse::isButtonPressed(sf::Mouse::Left);
		if (button == B_MIDDLE) return sf::Mouse::isButtonPressed(sf::Mouse::Middle);
		if (button == B_RIGHT) return sf::Mouse::isButtonPressed(sf::Mouse::Right);
		return false;
	}
	
	bool pressing(Key key, sf::Keyboard::Key code)
	{
		if (key == K_UP) return code == sf::Keyboard::W || code == sf::Keyboard::Up;
		if (key == K_DOWN) return code == sf::Keyboard::S || code == sf::Keyboard::Down;
		if (key == K_LEFT) return code == sf::Keyboard::A || code == sf::Keyboard::Left;
		if (key == K_RIGHT) return code == sf::Keyboard::D || code == sf::Keyboard::Right;
		return false;
	}

	bool pressing(Key key)
	{
		if (key == K_UP) return sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		if (key == K_DOWN) return sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		if (key == K_LEFT) return sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		if (key == K_RIGHT) return sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		return false;
	}
}
