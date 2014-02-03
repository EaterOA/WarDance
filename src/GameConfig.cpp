#include "GameConfig.hpp"
#include <assert.h>
#include <fstream>

GameConfig config;

GameConfig::GameConfig()
{
}

bool GameConfig::loadConfig(const std::string& path)
{
	std::fstream fin(path.c_str());
	if (fin.bad()) return false;
	std::string param;
	int value;
	while (fin >> param >> value) db_int[param] = value;
	fin.close();

	db_int["level"] = 1;
	db_int["num_levels"] = 5;
	db_int["highscore"] = 0;
	return true;
}

int GameConfig::getInt(const std::string& key) const
{
    std::map<std::string, int>::const_iterator iter = db_int.find(key);
    assert(iter != db_int.end());
    return iter->second;
}


int GameConfig::getInt(const std::string& key, int defaultValue) const
{
    std::map<std::string, int>::const_iterator iter = db_int.find(key);
    return (iter != db_int.end() ? iter->second : defaultValue);
}

void GameConfig::setInt(const std::string& key, int value)
{
    db_int[key] = value;
}

bool GameConfig::clicking(Button button)
{
	if (button == B_LEFT) return sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (button == B_MIDDLE) return sf::Mouse::isButtonPressed(sf::Mouse::Middle);
	if (button == B_RIGHT) return sf::Mouse::isButtonPressed(sf::Mouse::Right);
	return false;
}
	
bool GameConfig::pressing(Key key, sf::Keyboard::Key code)
{
	if (key == K_UP) return code == sf::Keyboard::W || code == sf::Keyboard::Up;
	if (key == K_DOWN) return code == sf::Keyboard::S || code == sf::Keyboard::Down;
	if (key == K_LEFT) return code == sf::Keyboard::A || code == sf::Keyboard::Left;
	if (key == K_RIGHT) return code == sf::Keyboard::D || code == sf::Keyboard::Right;
	return false;
}

bool GameConfig::pressing(Key key)
{
	if (key == K_UP) return sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	if (key == K_DOWN) return sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	if (key == K_LEFT) return sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	if (key == K_RIGHT) return sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	return false;
}