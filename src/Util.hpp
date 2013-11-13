#ifndef UTIL_HPP
#define UTIL_HPP

#include <SFML/Graphics.hpp>

struct GameState;

namespace util
{
	const float PI = 3.14159265f;

	float getDist(const sf::Vector2f &p1, const sf::Vector2f &p2);
	bool inMap(const sf::Vector2f &p, const GameState& state);
	bool hasCollided(const sf::FloatRect &r1, const sf::FloatRect &r2);
	float getHyp(const sf::Vector2f &vec);
	bool insideCircle(const sf::Vector2f &seg, const sf::Vector2f &cpos, float rad);
	bool hasCollided(const sf::FloatRect &r, const sf::Vector2f &cpos, float rad);
}

#endif