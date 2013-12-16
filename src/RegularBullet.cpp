#include "Actors.hpp"
#include "GameMechanics.hpp"
#include <math.h>

RegularBullet::RegularBullet(sf::Vector2f pos, float dir, int faction)
	: Projectile(5, pos, util::ShapeVector(util::Rectangle, 10, 2), 5, 5, faction)
{
	m_dir = dir;
	m_vel.x = 100.f * cos(m_dir);
	m_vel.y = 100.f * sin(m_dir);
}

RegularBullet::RegularBullet(sf::Vector2f pos, float dir, float normX, float normY, int faction)
	: Projectile(5, pos, util::ShapeVector(util::Rectangle, 5, 2), 5, 5, faction)
{
	m_dir = dir;
	m_vel.x = 100.f * normX;
	m_vel.y = 100.f * normY;
}

void RegularBullet::act(GameState &state)
{
	Actor::act(state);
	attack(state);
}