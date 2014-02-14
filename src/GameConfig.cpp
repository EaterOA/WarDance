#include "GameConfig.hpp"

GameConfig config;

bool GameConfig::init()
{
    std::map<std::string, std::string> defaults;
    defaults["hitbox_enabled"] = "0";
    defaults["bind_enter"] = "KEY_ENTER";
    defaults["bind_esc"] = "KEY_ESC";
    defaults["bind_up"] = "KEY_W";
    defaults["bind_down"] = "KEY_S";
    defaults["bind_left"] = "KEY_A";
    defaults["bind_right"] = "KEY_D";

    load("config/config.txt", defaults);

    std::map<std::string, std::string> overrides;
    overrides["level"] = "1";
    overrides["num_levels"] = "5";
    overrides["highscore"] = "0";

    load(overrides);

    return true;
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
