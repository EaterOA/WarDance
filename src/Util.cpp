#include "Util.hpp"
#include "GameMechanics.hpp"
#include <math.h>

namespace util
{
	float dot(sf::Vector2f v1, sf::Vector2f v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	float getLen(sf::Vector2f v)
	{
		return sqrt(dot(v,v));
	}

	float getMaxRad(sf::FloatRect r)
	{
		return getLen(sf::Vector2f(r.width/2, r.height/2));
	}

	float getDist(sf::Vector2f p1, sf::Vector2f p2)
	{
		return getLen(sf::Vector2f(p2.x-p1.x, p2.y-p1.y));
	}
	
	float getDist(sf::Vector2f p, sf::Vector2f p1, sf::Vector2f p2)
	{
		sf::Vector2f v(p2.x-p1.x, p2.y-p1.y);
		sf::Vector2f w(p.x-p1.x, p.y-p1.y);

		float dist, len;
		if ((dist = dot(w,v)) <= 0) return getDist(p, p1);
		if ((len = dot(v,v)) <= dist) return getDist(p, p2);
		float b = dist / len;
		sf::Vector2f pb(p1.x + b * v.x, p1.y + b * v.y);
		return getDist(p, pb);
	}

	bool hasCollided(const sf::FloatRect &r1, const sf::FloatRect &r2)
	{
		return !((r1.left > r2.left + r2.width) ||
				 (r1.top > r2.top + r2.height)  ||
			 	 (r2.left > r1.left + r1.width) ||
			 	 (r2.top > r1.top + r1.height)  );
	}

	bool hasCollided(const sf::FloatRect &r, float dir, const sf::Vector2f &cpos, float rad)
	{
		sf::Vector2f center(r.left + r.width/2, r.top + r.height/2);
		if (getMaxRad(r) + rad < getDist(center, cpos)) return false;

		float normX = cos(dir);
		float normY = sin(dir);
		sf::Vector2f rotWidth(normX * r.width, normY * r.width);
		sf::Vector2f rotHeight(normY * r.height, normX * r.height);
		sf::Vector2f topLeft(center.x - rotWidth.x/2 - rotHeight.x/2, center.y - rotWidth.y/2 - center.y/2);
		sf::Vector2f topRight(center.x + rotWidth.x/2 - rotHeight.x/2, center.y + rotWidth.y/2 - center.y/2);
		sf::Vector2f bottomRight(center.x + rotWidth.x/2 + rotHeight.x/2, center.y + rotWidth.y/2 + center.y/2);
		sf::Vector2f bottomLeft(center.x - rotWidth.x/2 + rotHeight.x/2, center.y - rotWidth.y/2 + center.y/2);
		if (getDist(cpos, topLeft, topRight) < rad) return true;
		if (getDist(cpos, topRight, bottomRight) < rad) return true;
		if (getDist(cpos, bottomRight, bottomLeft) < rad) return true;
		if (getDist(cpos, bottomLeft, topLeft) < rad) return true;
		return false;
	}

	bool hasCollided(const sf::FloatRect &r1, float dir1, const sf::FloatRect &r2, float dir2)
	{
		return false;
	}

	bool inMap(const sf::Vector2f &p, const GameState &state)
	{
		return p.x >= 0 && p.x <= state.map_width && p.y >= 0 && p.y <= state.map_height;
	}
}
