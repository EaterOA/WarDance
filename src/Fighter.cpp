#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"

Fighter::Fighter(int type, sf::Vector2f size, sf::Vector2f pos, int hp, int faction): Actor(type, pos)
{
	m_size = size;
	m_hp = hp;
	m_faction = faction;
}

int Fighter::getHP() const
{
	return m_hp;
}

sf::Vector2f Fighter::getSize()
{
	return m_size;
}

void Fighter::hit(int damage)
{
	m_hp -= damage;
}

void Fighter::cooldown()
{
	m_attack_cd--;
	if (m_attack_cd < -100) m_attack_cd = 0;
}

bool Fighter::isDead()
{
	return m_hp <= 0;
}

void Fighter::shootTowards(GameState &state, sf::Vector2f &dest, float offsetX, float offsetY)
{
	float dist, normX, normY, dir, rotX, rotY, outX, outY;

	dist = Util::getDist(m_pos, dest);
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
	if (offsetX > 0) {
		outX = normX * offsetX;
		outY = normY * offsetX;
	}
	else {
		outX = outY = 0;
	}
	Projectile* bullet = new RegularBullet(sf::Vector2f(m_pos.x + outX + rotX, m_pos.y + outY + rotY), m_faction);
	bullet->setVel(sf::Vector2f(normX * 100.f, normY * 100.f));
	bullet->setDir(dir);
	state.projectiles.push_back(bullet);
}