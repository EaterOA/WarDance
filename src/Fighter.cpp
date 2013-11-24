#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"

Fighter::Fighter(int type, sf::Vector2f size, sf::Vector2f pos, int hp, int faction): Actor(type, pos)
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

sf::Vector2f Fighter::getSize() const
{
	return m_size;
}

void Fighter::hit(int damage, GameState &state)
{
	m_hp -= damage;
}

void Fighter::cooldown()
{
	m_attack_cd--;
	if (m_attack_cd < -100) m_attack_cd = 0;
}

bool Fighter::isDead(const GameState &state)
{
	return m_hp <= 0;
}

void Fighter::shoot(GameState &state, float offsetX, float offsetY)
{
	float normX, normY, rotX, rotY, outX, outY;
	
	if (offsetX != 0 || offsetY != 0) {
		normX = std::cos(m_dir);
		normY = std::sin(m_dir);
		rotX = normY * -1 * offsetY;
		rotY = normX * offsetY;
		outX = normX * offsetX;
		outY = normY * offsetX;
	}
	else {
		outX = outY = rotX = rotY = 0;
	}
	Projectile* bullet = new RegularBullet(sf::Vector2f(m_pos.x + outX + rotX, m_pos.y + outY + rotY), m_dir, m_faction);
	state.projectiles.push_back(bullet);
}

void Fighter::shootTowards(GameState &state, sf::Vector2f &dest, float offsetX, float offsetY)
{
	float dist, normX, normY, dir, rotX, rotY, outX, outY;

	dist = util::getDist(m_pos, dest);
	normX = (dest.x - m_pos.x) / dist;
	normY = (dest.y - m_pos.y) / dist;
	dir = std::atan(normY / normX) * 57.29578f;
	if (normX < 0) dir += 180;
	if (offsetY > 0) {
		rotX = std::sin(m_dir / 57.29578f) * -1 * offsetY;
		rotY = std::cos(m_dir / 57.29578f) * offsetY;
	}
	else {
		rotX = rotY = 0;
	}
	outX = normX * offsetX;
	outY = normY * offsetX;
	Projectile* bullet = new RegularBullet(sf::Vector2f(m_pos.x + outX + rotX, m_pos.y + outY + rotY), dir, m_faction);
	state.projectiles.push_back(bullet);
}