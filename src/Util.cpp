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

	float getMaxRad(sf::Vector2f s)
	{
		return getLen(sf::Vector2f(s.x/2, s.y/2));
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

	bool hasCollided(sf::Vector2f c1, util::ShapeVector s1, float dir1, sf::Vector2f c2, util::ShapeVector s2, float dir2)
	{
		if (s1.s == Circle && s2.s == Circle) return hasCollided(c1, s1.x, c2, s2.x);
		if (s1.s == Rectangle && s2.s == Circle) return hasCollided(c1, sf::Vector2f(s1.x, s1.y), dir1, c2, s2.x);
		if (s1.s == Circle && s2.s == Rectangle) return hasCollided(c2, sf::Vector2f(s2.x, s2.y), dir2, c1, s1.x);
		if (s1.s == Rectangle && s2.s == Rectangle) return hasCollided(c1, sf::Vector2f(s1.x, s1.y), dir1, c2, sf::Vector2f(s2.x, s2.y), dir2);
		return false;
	}

	bool hasCollided(sf::Vector2f c1, float r1, sf::Vector2f c2, float r2)
	{
		if (r1 + r2 < getDist(c1, c2)) return false;
		return true;
	}

	bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, float r2)
	{
		//Basic distance check
		if (getMaxRad(s1) + r2 < getDist(c1, c2)) return false;

		//Transforming vectors
		float normX1 = cos(dir1);
		float normY1 = sin(dir1);
		sf::Vector2f rotWidth1(normX1 * s1.x / 2.f, normY1 * s1.x / 2.f);
		sf::Vector2f rotHeight1(-normY1 * s1.y / 2.f, normX1 * s1.y / 2.f);
		sf::Vector2f tr1[] = {c1 - rotWidth1 - rotHeight1,
							  c1 + rotWidth1 - rotHeight1,
							  c1 + rotWidth1 + rotHeight1,
							  c1 - rotWidth1 + rotHeight1};

		//Inner check
		float si1 = crossZ(tr1[1] - tr1[0], c2 - tr1[0]);
		float si2 = crossZ(tr1[2] - tr1[1], c2 - tr1[1]);
		float si3 = crossZ(tr1[3] - tr1[2], c2 - tr1[2]);
		float si4 = crossZ(tr1[0] - tr1[3], c2 - tr1[3]);
		if (si1 < 0 && si2 < 0 && si3 < 0 && si4 < 0) return true;
		if (si1 > 0 && si2 > 0 && si3 > 0 && si4 > 0) return true;
		return false;

		//Edge cross check
		if (getDist(c2, tr1[0], tr1[1]) < r2) return true;
		if (getDist(c2, tr1[1], tr1[2]) < r2) return true;
		if (getDist(c2, tr1[2], tr1[3]) < r2) return true;
		if (getDist(c2, tr1[3], tr1[0]) < r2) return true;

		return false;
	}

	bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, sf::Vector2f s2, float dir2)
	{
		//Basic distance check
		if (getMaxRad(s1) + getMaxRad(s2) < getDist(c1, c2)) return false;
		
		//Transforming vectors
		float normX1 = cos(dir1);
		float normY1 = sin(dir1);
		sf::Vector2f rotWidth1(normX1 * s1.x / 2.f, normY1 * s1.x / 2.f);
		sf::Vector2f rotHeight1(-normY1 * s1.y / 2.f, normX1 * s1.y / 2.f);
		sf::Vector2f tr1[] = {c1 - rotWidth1 - rotHeight1,
							  c1 + rotWidth1 - rotHeight1,
							  c1 + rotWidth1 + rotHeight1,
							  c1 - rotWidth1 + rotHeight1};
		float normX2 = cos(dir2);
		float normY2 = sin(dir2);
		sf::Vector2f rotWidth2(normX2 * s2.x / 2.f, normY2 * s2.x / 2.f);
		sf::Vector2f rotHeight2(-normY2 * s2.y / 2.f, normX2 * s2.y / 2.f);
		sf::Vector2f tr2[] = {c2 - rotWidth2 - rotHeight2,
							  c2 + rotWidth2 - rotHeight2,
							  c2 + rotWidth2 + rotHeight2,
							  c2 - rotWidth2 + rotHeight2};

		//Axis separation theorem
		for (int i = 0; i < 4; i++) {
			sf::Vector2f separator = tr1[(i+1)%4] - tr1[i];
			float side = crossZ(tr1[(i+2)%4] - tr1[i], separator);
			int sign = (side > 0? -1 : 1);
			if (crossZ(tr2[0]-tr1[i], separator) * sign > 0 &&
				crossZ(tr2[1]-tr1[i], separator) * sign > 0 &&
				crossZ(tr2[2]-tr1[i], separator) * sign > 0 &&
				crossZ(tr2[3]-tr1[i], separator) * sign > 0)
				return false;
		}
		for (int i = 0; i < 4; i++) {
			sf::Vector2f separator = tr2[(i+1)%4] - tr2[i];
			float side = crossZ(tr2[(i+2)%4] - tr2[i], separator);
			int sign = (side > 0? -1 : 1);
			if (crossZ(tr1[0]-tr2[i], separator) * sign > 0 &&
				crossZ(tr1[1]-tr2[i], separator) * sign > 0 &&
				crossZ(tr1[2]-tr2[i], separator) * sign > 0 &&
				crossZ(tr1[3]-tr2[i], separator) * sign > 0)
				return false;
		}

		return true;
	}
}
