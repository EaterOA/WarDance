#include "Config.hpp"
#include "Util.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>

Config config;

void Config::parse(std::istream& in)
{
    std::string line;
    while (getline(in, line)) {
        std::size_t idx = line.find("=");
        if (idx != std::string::npos) {
            std::string key = util::trim(line.substr(0, idx));
            std::string value = util::trim(line.substr(idx+1));
            db_str[key] = value;
        }
    }
}

bool Config::load(const std::string& path, const std::map<std::string, std::string>& defaultValues /*= std::map<std::string, std::string>()*/)
{
	std::fstream fin(path.c_str());
	if (fin.bad()) return false;
    parse(fin);
	db_str["level"] = "1";
	db_str["num_levels"] = "5";
	db_str["highscore"] = "0";
	return true;
}

bool Config::load(const std::vector<std::string>& paths, const std::map<std::string, std::string>& defaultValues /*= std::map<std::string, std::string>()*/)
{
    bool flag = false;
    for (unsigned i = 0; i < paths.size(); i++) {
        std::fstream fin(paths[i].c_str());
        if (fin.bad()) continue;
        flag = true;
        parse(fin);
    }
    return flag;
}

bool Config::load(const std::map<std::string, std::string>& db)
{
    for (std::map<std::string, std::string>::const_iterator iter = db.begin(); iter != db.end(); iter++) {
        db_str[iter->first] = iter->second;
    }
    return true;
}

std::string Config::getStr(const std::string& key)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    assert(iter != db_str.end());
    return iter->second;
}

std::string Config::getStr(const std::string& key, const std::string& defaultValue)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    return "";
    return iter != db_str.end() ? iter->second : defaultValue;
}

void Config::setStr(const std::string& key, const std::string& value)
{
    db_str[key] = value;
}

int Config::getInt(const std::string& key)
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


int Config::getInt(const std::string& key, int defaultValue)
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

void Config::setInt(const std::string& key, int value)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    if (iter != db_str.end()) db_str.erase(iter);
    db_int[key] = value;
}

bool Config::clicking(Button button)
{
	if (button == B_LEFT) return sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (button == B_MIDDLE) return sf::Mouse::isButtonPressed(sf::Mouse::Middle);
	if (button == B_RIGHT) return sf::Mouse::isButtonPressed(sf::Mouse::Right);
	return false;
}
	
bool Config::pressing(Key key, sf::Keyboard::Key code)
{
	if (key == K_UP) return code == sf::Keyboard::W || code == sf::Keyboard::Up;
	if (key == K_DOWN) return code == sf::Keyboard::S || code == sf::Keyboard::Down;
	if (key == K_LEFT) return code == sf::Keyboard::A || code == sf::Keyboard::Left;
	if (key == K_RIGHT) return code == sf::Keyboard::D || code == sf::Keyboard::Right;
	return false;
}

bool Config::pressing(Key key)
{
	if (key == K_UP) return sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	if (key == K_DOWN) return sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	if (key == K_LEFT) return sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	if (key == K_RIGHT) return sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	return false;
}
