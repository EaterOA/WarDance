#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)
#define RAND(min,max) (rand() % (max - min + 1) + min)

namespace util
{
	//Geometry
	enum Shape { Circle, Rectangle, Stroke };
	struct ShapeVector
	{
		ShapeVector() {};
		ShapeVector(Shape ss, float r): s(ss), x(r), y(r) {};
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
	float getMinRad(sf::Vector2f s);
	float getDist(sf::Vector2f p1, sf::Vector2f p2);
	float getDist(sf::Vector2f p, sf::Vector2f p1, sf::Vector2f p2);

	//Collision physics
	bool hasCollided(sf::Vector2f c1, util::ShapeVector s1, float dir1, sf::Vector2f c2, util::ShapeVector s2, float dir2);
    bool hasCollided(sf::Vector2f c1, float r11, float r12, sf::Vector2f c2, float r2);
    bool hasCollided(sf::Vector2f c1, float r11, float r12, sf::Vector2f c2, sf::Vector2f s2, float dir2);
	bool hasCollided(sf::Vector2f c1, float r1, sf::Vector2f c2, float r2);
	bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, float r2);
	bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, sf::Vector2f s2, float dir2);

	//Sprite manipulation 
	void copySprite(const sf::Vertex src[4], sf::Vertex dest[4]);
	void copyTexture(const sf::Vertex src[4], sf::Vertex dest[4]);
	void translateSprite(sf::Vertex sprite[4], sf::Vector2f translation);
	void affixPos(sf::Vertex sprite[4], sf::Vector2f pos, sf::Vector2f size, int reference);
	void affixTexture(sf::Vertex sprite[4], sf::Vector2f texCoord, sf::Vector2f size);
	void rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f center);
	void applyColor(sf::Vertex sprite[4], unsigned int rgba);
	void setAlpha(sf::Vertex sprite[4], unsigned char alpha);
	sf::Color toColor(unsigned int rgba);
	
	//IO convenience
	bool readf(std::istream &in, unsigned amt, float arr[], bool endline);
	bool readv2f(std::istream &in, sf::Vector2f &dest, bool endline);
	bool read3v2f(std::istream &in, sf::Vector2f &dest1, sf::Vector2f &dest2, sf::Vector2f &dest3, bool endline);

	//String manipulation
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
	bool isPrefix(const std::string &p, const std::string &s);
}

#endif
