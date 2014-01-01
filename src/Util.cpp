#include "Util.hpp"

namespace util
{

	int rand(int min, int max)
	{
		int range = max - min + 1;
		return std::rand() % range + min;
	}

	float toDir(float vx, float vy)
	{
		return atan(vy / vx) + (vx < 0 ? PI : 0);
	}

	float toRad(float deg)
	{
		return deg / 180.f * util::PI;
	}

	float toDeg(float rad)
	{
		return rad * 180.f / util::PI;
	}

	float rotateRad(float rad, float amt)
	{
		rad += amt;
		while (rad < 0) rad += 2*PI;
		while (rad > 2*PI) rad -= 2*PI;
		return rad;
	}

	float rotateDeg(float deg, float amt)
	{
		deg += amt;
		while (deg < 0) deg += 360;
		while (deg > 360) deg -= 360;
		return deg;
	}

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

	float getMinRad(sf::Vector2f s)
	{
		return MIN(s.x / 2, s.y / 2);
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
		return (r1 + r2 > getDist(c1, c2));
	}

	bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, float r2)
	{
		//Basic distance check
		float dist = getDist(c1, c2);
		if (getMaxRad(s1) + r2 < dist) return false;
		if (getMinRad(s1) + r2 > dist) return true;

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
		float dist = getDist(c1, c2);
		if (getMaxRad(s1) + getMaxRad(s2) < dist) return false;
		if (getMinRad(s1) + getMinRad(s2) > dist) return true;

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

	bool readf(std::istream &in, unsigned amt, float arr[], bool endline)
	{
		for (unsigned i = 0; i < amt && in >> arr[i]; i++);
		if (endline) in.ignore(1000, '\n');
		return !in.fail();
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
	{
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	std::vector<std::string> split(const std::string &s, char delim) 
	{
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

}
