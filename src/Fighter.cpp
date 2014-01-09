#include "Actors.hpp"
#include "GameMechanics.hpp"
#include <math.h>

Fighter::Fighter(std::string frame, util::ShapeVector size, sf::Vector2f pos, int hp, int faction)
	: Actor(frame, pos, size)
{
	m_attack_cd = 0;
	m_hp = hp;
	m_maxHp = hp;
	m_faction = faction;
}

int Fighter::getMaxHP() const
{
	return m_maxHp;
}

int Fighter::getHP() const
{
	return m_hp;
}

int Fighter::getFaction() const
{
	return m_faction;
}

void Fighter::hit(GameState &state, int damage)
{
	m_hp -= damage;
}

void Fighter::cooldown(GameState &state)
{
	m_attack_cd -= state.elapsed.asMilliseconds();
	if (m_attack_cd < -5000) m_attack_cd = 0;
}

bool Fighter::isDead(GameState &state)
{
	return m_hp <= 0;
}

void Fighter::shoot(GameState &state, Bullet b, float offsetX, float offsetY)
{
	shoot(state, b, m_dir, offsetX, offsetY);
}

void Fighter::shoot(GameState &state, Bullet b, sf::Vector2f &dest, float offsetX, float offsetY)
{
	float dist = util::getDist(m_pos, dest);
	float unitX = (dest.x - m_pos.x) / dist;
	float unitY = (dest.y - m_pos.y) / dist;
	float dir = util::toDir(unitX, unitY);
	shoot(state, b, dir, unitX, unitY, offsetX, offsetY);
}

void Fighter::shoot(GameState &state, Bullet b, float dir, float offsetX, float offsetY)
{
	float unitX = cos(dir);
	float unitY = sin(dir);
	shoot(state, b, dir, unitX, unitY, offsetX, offsetY);
}

void Fighter::shoot(GameState &state, Bullet b, float dir, float extraDir, float offsetX, float offsetY)
{
	float unitX = cos(dir);
	float unitY = sin(dir);
	shoot(state, b, dir + extraDir, unitX, unitY, offsetX, offsetY);
}

void Fighter::shoot(GameState &state, Bullet b, float dir, float unitX, float unitY, float offsetX, float offsetY)
{
	float rotX, rotY, outX, outY;

	if (offsetX != 0 || offsetY != 0) {
		rotX = unitY * -1 * offsetY;
		rotY = unitX * offsetY;
		outX = unitX * offsetX;
		outY = unitY * offsetX;
	}
	else {
		outX = outY = rotX = rotY = 0;
	}
    Projectile* bullet;
    if (b == SPLITTING) 
	    bullet = new SplittingBullet(sf::Vector2f(m_pos.x + outX + rotX, m_pos.y + outY + rotY), dir, m_faction);
	else if(b == LASER)
		bullet = new Laser(sf::Vector2f(m_pos.x + outX + rotX, m_pos.y + outY + rotY), dir, m_faction);
	else
	    bullet = new RegularBullet(sf::Vector2f(m_pos.x + outX + rotX, m_pos.y + outY + rotY), dir, m_faction);
	state.projectiles.push_back(bullet);
}
