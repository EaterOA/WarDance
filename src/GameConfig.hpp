#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class GameConfig
{
public:
    enum Key {K_UP, K_DOWN, K_LEFT, K_RIGHT};
    enum Button {B_LEFT, B_MIDDLE, B_RIGHT};

    GameConfig();
    bool loadConfig(const std::string& path);
    int getInt(const std::string& key) const;
    int getInt(const std::string& key, int defaultValue) const;
    void setInt(const std::string& key, int value);
    bool clicking(Button button);
    bool pressing(Key key, sf::Keyboard::Key code);
    bool pressing(Key key);
private:
    std::map<std::string, int> db_int;
};

extern GameConfig config;

#endif
