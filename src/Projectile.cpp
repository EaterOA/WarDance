#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"

Projectile::Projectile(int type, sf::Vector2f pos, float radius, int damage, int faction): Fighter(type, sf::Vector2f(), pos, damage, faction)
{
	m_radius = radius;
	m_faction = faction;
}

void Projectile::setVel(sf::Vector2f &v)
{
	m_vel = v;
}

void Projectile::setDir(float dir)
{
	m_dir = dir;
}