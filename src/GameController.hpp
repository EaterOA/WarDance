#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <SFML/Graphics.hpp>

class GameController
{
public:
    enum Key {K_ENTER, K_ESCAPE, K_UP, K_DOWN, K_LEFT, K_RIGHT};
    enum Button {B_LEFT, B_MIDDLE, B_RIGHT};

    bool init();
    bool updateBinding();
    bool clicking(Button button);
    bool pressing(Key key, sf::Keyboard::Key code);
    bool pressing(Key key);
private:
    void initKeycodeMap();

    std::map<std::string, sf::Keyboard::Key> keycode_map;
    std::map<Key, sf::Keyboard::Key> key_map;
    std::map<Button, sf::Mouse::Button> button_map;
};

extern GameController controller;

#endif
