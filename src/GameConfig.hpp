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

    std::map<std::string, std::string> parse(std::istream& in);
    bool loadConfig(const std::string& path);
    std::string getStr(const std::string& key);
    std::string getStr(const std::string& key, const std::string &defaultValue);
    void setStr(const std::string& key, const std::string &value);
    int getInt(const std::string& key);
    int getInt(const std::string& key, int defaultValue);
    void setInt(const std::string& key, int value);
    bool clicking(Button button);
    bool pressing(Key key, sf::Keyboard::Key code);
    bool pressing(Key key);
private:
    std::map<std::string, int> db_int;
    std::map<std::string, std::string> db_str;
};

extern GameConfig config;

#endif
