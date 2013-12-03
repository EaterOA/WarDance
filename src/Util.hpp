#ifndef UTIL_HPP
#define UTIL_HPP

#include <SFML/Graphics.hpp>

namespace util
{
	const float PI = 3.14159265f;

	float dot(sf::Vector2f v1, sf::Vector2f v2);
	float crossZ(sf::Vector2f v1, sf::Vector2f v2);
	float getLen(sf::Vector2f v);
	float getMaxRad(sf::FloatRect r);
	float getDist(sf::Vector2f p1, sf::Vector2f p2); //Distance between two points
	float getDist(sf::Vector2f p, sf::Vector2f p1, sf::Vector2f p2); //Distance between point and segment
	bool hasCollided(const sf::FloatRect &r1, const sf::FloatRect &r2);
	bool hasCollided(const sf::FloatRect &r, float dir, const sf::Vector2f &cpos, float rad);
	bool hasCollided(const sf::FloatRect &r1, float dir1, const sf::FloatRect &r2, float dir2);
}

#endif