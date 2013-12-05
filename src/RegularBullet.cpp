#include "Actors.hpp"
#include "GameMechanics.hpp"
#include <math.h>

RegularBullet::RegularBullet(sf::Vector2f pos, float dir, int faction)
	: Projectile(5, pos, util::ShapeVector(util::Rectangle, 5, 2), 5, 5, faction)
{
	m_dir = dir;
	m_vel.x = 100.f * cos(m_dir);
	m_vel.y = 100.f * sin(m_dir);
}

void RegularBullet::act(GameState &state)
{
	Actor::act(state);
	attack(state);
}

bool RegularBullet::isDead(const GameState &state)
{
	return m_hp <= 0 || !state.inMap(m_pos);
}

void RegularBullet::attack(GameState& state)
{
	if (util::hasCollided(state.player->getPos(), state.player->getSize(), state.player->getDir(), m_pos, m_size, m_dir)) {
		state.player->hit(m_damage, state);
		hit(m_damage, state);
	}
	else if (m_faction == 0) {
		for (unsigned i = 0; i < state.enemies.size(); i++) {
			if (util::hasCollided(state.enemies[i]->getPos(), state.enemies[i]->getSize(), state.enemies[i]->getDir(), m_pos, m_size, m_dir)) {
				state.enemies[i]->hit(m_damage, state);
				hit(m_damage, state);
				break;
			}
		}
	}
}
