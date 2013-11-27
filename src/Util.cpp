#include "Util.hpp"
#include "GameMechanics.hpp"
#include <math.h>

namespace util
{
	float getDist(const sf::Vector2f &p1, const sf::Vector2f &p2)
	{
		return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
	}

	bool inMap(const sf::Vector2f &p, const GameState &state)
	{
		return p.x >= 0 && p.x <= state.map_width && p.y >= 0 && p.y <= state.map_height;
	}

	bool hasCollided(const sf::FloatRect &r1, const sf::FloatRect &r2)
	{
		return !((r1.left > r2.left + r2.width) ||
					(r1.top > r2.top + r2.height)  ||
					(r2.left > r1.left + r1.width) ||
					(r2.top > r1.top + r1.height)  );
	}

	float getHyp(const sf::Vector2f &vec)
	{
		return sqrt(vec.x*vec.x + vec.y*vec.y);
	}

	bool insideCircle(const sf::Vector2f &seg, const sf::Vector2f &cpos, float rad)
	{
		return false;
	}

	bool hasCollided(const sf::FloatRect &r, const sf::Vector2f &cpos, float rad)
	{
		bool res = r.contains(cpos);
		return res;
		//if (
		/*return !((r1.left > r2.left + r2.width) ||
					(r1.top > r2.top + r2.height)  ||
					(r2.left > r1.left + r1.width) ||
					(r2.top > r1.top + r1.height)  );*/
	}
}
