#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

extern std::map<std::string, int> config;

namespace conf
{
	enum Key {UP, DOWN, LEFT, RIGHT};

	bool init_config(std::string path);
	bool pressing(Key key, sf::Keyboard::Key code);
	bool pressing(Key key);
}

#endif
