#include "Actors.hpp"
#include "GameMechanics.hpp"

Projectile::Projectile(int type, sf::Vector2f pos, util::ShapeVector size, int hp, int damage, int faction)
	: Fighter(type, size, pos, hp, faction)
{
	m_damage = damage;
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