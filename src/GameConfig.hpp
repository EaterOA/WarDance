#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP

#include "Config.hpp"
#include <SFML/Graphics.hpp>

class GameConfig: public Config
{
public:
    enum Key {K_UP, K_DOWN, K_LEFT, K_RIGHT};
    enum Button {B_LEFT, B_MIDDLE, B_RIGHT};

    bool init();    
    bool clicking(Button button);
    bool pressing(Key key, sf::Keyboard::Key code);
    bool pressing(Key key);
};

extern GameConfig config;

#endif
