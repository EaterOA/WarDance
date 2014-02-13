#include "GameConfig.hpp"
#include "Util.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>

GameConfig config;

std::map<std::string, std::string> GameConfig::parse(std::istream& in)
{
    std::map<std::string, std::string> c;
    std::string line;
    while (getline(in, line)) {
        std::size_t idx = line.find("=");
        if (idx != std::string::npos) {
            std::string key = line.substr(0, idx);
            key = util::trim(key);
            std::string value = line.substr(idx+1);
            value = util::trim(value);
            c[key] = value;
        }
    }
    return c;
}

bool GameConfig::loadConfig(const std::string& path)
{
	std::fstream fin(path.c_str());
	if (fin.bad()) return false;
    db_str = parse(fin);
	fin.close();

	db_str["level"] = "1";
	db_str["num_levels"] = "5";
	db_str["highscore"] = "0";
	return true;
}

std::string GameConfig::getStr(const std::string& key)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    assert(iter != db_str.end());
    return iter->second;
}

std::string GameConfig::getStr(const std::string& key, const std::string& defaultValue)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    return "";
    return iter != db_str.end() ? iter->second : defaultValue;
}

void GameConfig::setStr(const std::string& key, const std::string& value)
{
    db_str[key] = value;
}

int GameConfig::getInt(const std::string& key)
{
    std::map<std::string, std::string>::iterator iter1 = db_str.find(key);
    if (iter1 != db_str.end()) {
        int val;
        assert(std::stringstream(iter1->second) >> val);
        db_int[key] = val;
        db_str.erase(iter1);
    }
    std::map<std::string, int>::const_iterator iter2 = db_int.find(key);
    assert(iter2 != db_int.end());
    return iter2->second;
}


int GameConfig::getInt(const std::string& key, int defaultValue)
{
    std::map<std::string, std::string>::iterator iter1 = db_str.find(key);
    if (iter1 != db_str.end()) {
        int val;
        assert(std::stringstream(iter1->second) >> val);
        db_int[key] = val;
        db_str.erase(iter1);
    }
    std::map<std::string, int>::iterator iter2 = db_int.find(key);
    return (iter2 != db_int.end() ? iter2->second : defaultValue);
}

void GameConfig::setInt(const std::string& key, int value)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    if (iter != db_str.end()) db_str.erase(iter);
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
