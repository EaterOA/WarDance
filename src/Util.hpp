#include <SFML/Graphics.hpp>
#include <math.h>

class Util
{
public:
	static const int MAP_WIDTH = 1600, MAP_HEIGHT= 1200;
	static float getDist(const sf::Vector2f &p1, const sf::Vector2f &p2)
	{
		return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
	}
	static bool inMap(const sf::Vector2f &p)
	{
		return p.x >= 0 && p.x <= MAP_WIDTH && p.y >= 0 && p.y <= MAP_HEIGHT;
	}
	static bool hasCollided(const sf::FloatRect &r1, const sf::FloatRect &r2)
	{
		return !((r1.left > r2.left + r2.width) ||
				 (r1.top > r2.top + r2.height)  ||
				 (r2.left > r1.left + r1.width) ||
				 (r2.top > r1.top + r1.height)  );
	}
	static float getHyp(const sf::Vector2f &vec)
	{
		return std::sqrt(vec.x*vec.x + vec.y*vec.y);
	}
	static bool insideCircle(const sf::Vector2f &seg, const sf::Vector2f &cpos, float rad)
	{
		return false;
	}
	static bool hasCollided(const sf::FloatRect &r, const sf::Vector2f &cpos, float rad)
	{
		bool res = r.contains(cpos);
		return res;
		//if (
		/*return !((r1.left > r2.left + r2.width) ||
				 (r1.top > r2.top + r2.height)  ||
				 (r2.left > r1.left + r1.width) ||
				 (r2.top > r1.top + r1.height)  );*/
	}
};