#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

namespace util
{
	enum Shape { Circle, Rectangle };

	struct ShapeVector
	{
		ShapeVector() {};
		ShapeVector(Shape ss, float r): s(ss), x(r), y(0) {};
		ShapeVector(Shape ss, float xx, float yy): s(ss), x(xx), y(yy) {};
		Shape s;
		float x, y;
	};

	const float PI = 3.14159265f;

	float toDir(float vx, float vy);
	float toRad(float deg);
	float toDeg(float rad);
	float rotateRad(float rad, float amt);
	float rotateDeg(float deg, float amt);
	float dot(sf::Vector2f v1, sf::Vector2f v2);
	float crossZ(sf::Vector2f v1, sf::Vector2f v2);
	float getLen(sf::Vector2f v);
	float getMaxRad(sf::Vector2f s);
	float getDist(sf::Vector2f p1, sf::Vector2f p2);
	float getDist(sf::Vector2f p, sf::Vector2f p1, sf::Vector2f p2);
	bool hasCollided(sf::Vector2f c1, util::ShapeVector s1, float dir1, sf::Vector2f c2, util::ShapeVector s2, float dir2);
	bool hasCollided(sf::Vector2f c1, float r1, sf::Vector2f c2, float r2);
	bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, float r2);
	bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, sf::Vector2f s2, float dir2);
}

#endif