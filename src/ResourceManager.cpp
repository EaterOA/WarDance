#include "ResourceManager.hpp"
#include <assert.h>

bool ResourceManager::loadTexture(const std::string& key, const std::string &path)
{
    return db_tex[key].loadFromFile(path);
}

bool ResourceManager::setTexture(const std::string& key, const sf::Texture& t)
{
    db_tex[key] = t;
    return true;
}

const sf::Texture& ResourceManager::getTexture(const std::string& key) const
{
    std::map<std::string, sf::Texture>::const_iterator iter = db_tex.find(key);
    assert(iter != db_tex.end());
    return iter->second;
}

bool ResourceManager::loadFont(const std::string& key, const std::string &path)
{
    return db_font[key].loadFromFile(path);
}

bool ResourceManager::setFont(const std::string& key, const sf::Font& f)
{
    db_font[key] = f;
    return true;
}

const sf::Font& ResourceManager::getFont(const std::string& key) const
{
    std::map<std::string, sf::Font>::const_iterator iter = db_font.find(key);
    assert(iter != db_font.end());
    return iter->second;
}