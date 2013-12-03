#include "Util.hpp"
#include <math.h>

namespace util
{
	float dot(sf::Vector2f v1, sf::Vector2f v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	float crossZ(sf::Vector2f v1, sf::Vector2f v2)
	{
		return v2.y * v1.x - v2.x * v1.y;
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

	bool inRectangle(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Vector2f p)
	{
		float s1 = crossZ(p2 - p1, p - p1);
		float s2 = crossZ(p3 - p2, p - p2);
		float s3 = crossZ(p4 - p3, p - p3);
		float s4 = crossZ(p1 - p4, p - p4);
		if (s1 < 0 && s2 < 0 && s3 < 0 && s4 < 0) return true;
		if (s1 > 0 && s2 > 0 && s3 > 0 && s4 > 0) return true;
		return false;
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
		//Basic distance check
		sf::Vector2f center(r.left + r.width/2, r.top + r.height/2);
		if (getMaxRad(r) + rad < getDist(center, cpos)) return false;

		//Transforming vectors
		float normX = cos(dir);
		float normY = sin(dir);
		sf::Vector2f rotWidth(normX * r.width / 2.f, normY * r.width / 2.f);
		sf::Vector2f rotHeight(-normY * r.height / 2.f, normX * r.height / 2.f);
		sf::Vector2f topLeft = center - rotWidth - rotHeight;
		sf::Vector2f topRight = center + rotWidth - rotHeight;
		sf::Vector2f bottomRight = center + rotWidth + rotHeight;
		sf::Vector2f bottomLeft = center - rotWidth + rotHeight;

		//Inner check
		if (inRectangle(topLeft, topRight, bottomRight, bottomLeft, cpos)) return true;

		//Edge cross check
		if (getDist(cpos, topLeft, topRight) < rad) return true;
		if (getDist(cpos, topRight, bottomRight) < rad) return true;
		if (getDist(cpos, bottomRight, bottomLeft) < rad) return true;
		if (getDist(cpos, bottomLeft, topLeft) < rad) return true;

		return false;
	}

	bool hasCollided(const sf::FloatRect &r1, float dir1, const sf::FloatRect &r2, float dir2)
	{
		sf::Vector2f c1(r1.left + r1.width/2, r1.top + r1.height/2);
		sf::Vector2f c2(r2.left + r2.width/2, r2.top + r2.height/2);
		if (getMaxRad(r1) + getMaxRad(r2) < getDist(c1, c2)) return false;
		
		//Transforming vectors
		float normX1 = cos(dir1);
		float normY1 = sin(dir1);
		sf::Vector2f rotWidth1(normX1 * r1.width / 2.f, normY1 * r1.width / 2.f);
		sf::Vector2f rotHeight1(-normY1 * r1.height / 2.f, normX1 * r1.height / 2.f);
		sf::Vector2f topLeft1 = c1 - rotWidth1 - rotHeight1;
		sf::Vector2f topRight1 = c1 + rotWidth1 - rotHeight1;
		sf::Vector2f bottomRight1 = c1 + rotWidth1 + rotHeight1;
		sf::Vector2f bottomLeft1 = c1 - rotWidth1 + rotHeight1;
		float normX2 = cos(dir2);
		float normY2 = sin(dir2);
		sf::Vector2f rotWidth2(normX2 * r2.width / 2.f, normY2 * r2.width / 2.f);
		sf::Vector2f rotHeight2(-normY2 * r2.height / 2.f, normX2 * r2.height / 2.f);
		sf::Vector2f topLeft2 = c2 - rotWidth2 - rotHeight2;
		sf::Vector2f topRight2 = c2 + rotWidth2 - rotHeight2;
		sf::Vector2f bottomRight2 = c2 + rotWidth2 + rotHeight2;
		sf::Vector2f bottomLeft2 = c2 - rotWidth2 + rotHeight2;
		
		return false;
	}
}
