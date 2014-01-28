#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

extern std::map<std::string, int> config;

namespace conf
{
    enum Key {K_UP, K_DOWN, K_LEFT, K_RIGHT};
    enum Button {B_LEFT, B_MIDDLE, B_RIGHT};

    bool init_config(std::string path);
    bool clicking(Button button);
    bool pressing(Key key, sf::Keyboard::Key code);
    bool pressing(Key key);
}

#endif
