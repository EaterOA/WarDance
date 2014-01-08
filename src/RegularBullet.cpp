#include "Actors.hpp"
#include "GameMechanics.hpp"

RegularBullet::RegularBullet(sf::Vector2f pos, float dir, int faction)
	: Projectile("reg_bullet", pos, util::ShapeVector(util::Rectangle, 10, 2), 5, 5, faction)
{
	m_dir = dir;
	m_vel.x = 100.f * cos(m_dir);
	m_vel.y = 100.f * sin(m_dir);
	if (faction == 0) m_frame = "reg_bullet_p";
}

RegularBullet::RegularBullet(sf::Vector2f pos, float dir, float normX, float normY, int faction)
	: Projectile("reg_bullet", pos, util::ShapeVector(util::Rectangle, 5, 2), 5, 5, faction)
{
	m_dir = dir;
	m_vel.x = 100.f * normX;
	m_vel.y = 100.f * normY;
	if (faction == 0) m_frame = "reg_bullet_p";
}

void RegularBullet::act(GameState &state)
{
	Actor::act(state);
	attack(state);
}
