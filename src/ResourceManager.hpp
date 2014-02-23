#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class ResourceManager
{
public:
    bool loadTexture(const std::string &key, const std::string &path);
    bool setTexture(const std::string &key, const sf::Texture &t);
    const sf::Texture& getTexture(const std::string &key) const;
    bool loadFont(const std::string &key, const std::string &path);
    bool setFont(const std::string &key, const sf::Font &f);
    const sf::Font& getFont(const std::string &key) const;
protected:
    std::map<std::string, sf::Texture> db_tex;
    std::map<std::string, sf::Font> db_font;
};

#endif