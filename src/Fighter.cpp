#include "Actors.hpp"
#include "GameMechanics.hpp"
#include <math.h>

Fighter::Fighter(int type, util::ShapeVector size, sf::Vector2f pos, int hp, int faction)
	: Actor(type, pos)
{
	m_attack_cd = 0;
	m_size = size;
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

util::ShapeVector Fighter::getSize() const
{
	return m_size;
}

void Fighter::hit(int damage, GameState &state)
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

void Fighter::shoot(GameState &state, float offsetX, float offsetY)
{
	shoot(state, m_dir, offsetX, offsetY);
}

void Fighter::shoot(GameState &state, sf::Vector2f &dest, float offsetX, float offsetY)
{
	float dist = util::getDist(m_pos, dest);
	float normX = (dest.x - m_pos.x) / dist;
	float normY = (dest.y - m_pos.y) / dist;
	float dir = util::toDir(normX, normY);
	shoot(state, dir, normX, normY, offsetX, offsetY);
}

void Fighter::shoot(GameState &state, float dir, float offsetX, float offsetY)
{
	float normX = cos(dir);
	float normY = sin(dir);
	shoot(state, dir, normX, normY, offsetX, offsetY);
}

void Fighter::shoot(GameState &state, float dir, float extraDir, float offsetX, float offsetY)
{
	float normX = cos(dir);
	float normY = sin(dir);
	shoot(state, dir + extraDir, normX, normY, offsetX, offsetY);
}

void Fighter::shoot(GameState &state, float dir, float normX, float normY, float offsetX, float offsetY)
{
	float rotX, rotY, outX, outY;

	if (offsetX != 0 || offsetY != 0) {
		rotX = normY * -1 * offsetY;
		rotY = normX * offsetY;
		outX = normX * offsetX;
		outY = normY * offsetX;
	}
	else {
		outX = outY = rotX = rotY = 0;
	}
	Projectile* bullet = new RegularBullet(sf::Vector2f(m_pos.x + outX + rotX, m_pos.y + outY + rotY), dir, m_faction);
	state.projectiles.push_back(bullet);
}
