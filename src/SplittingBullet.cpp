#include "Actors.hpp"
#include "GameMechanics.hpp"
#include <math.h>

SplittingBullet::SplittingBullet(sf::Vector2f pos, float dir, int hp, int faction)
	: Projectile(6, pos, util::ShapeVector(util::Circle, 3.f), hp, 20, faction)
{
	m_dir = dir;
	m_vel.x = 100.f * cos(m_dir);
	m_vel.y = 100.f * sin(m_dir);
}

void SplittingBullet::act(GameState &state)
{
	if (--m_hp <= 0) {
		for (int deg = 0; deg < 360; deg += 10) {
			float rad = deg / 180.f * util::PI;
			state.projectiles.push_back(new RegularBullet(m_pos, rad, m_faction));
		}
	}
	else {
		Actor::act(state);
		attack(state);
	}
}

bool SplittingBullet::isDead(const GameState &state)
{
	return m_hp <= 0 || !state.inMap(m_pos);
}

void SplittingBullet::attack(GameState& state)
{
	if (util::hasCollided(state.player->getPos(), state.player->getSize(), state.player->getDir(), m_pos, m_size, m_dir)) {
		state.player->hit(m_damage, state);
		m_hp = 0;
	}
	else if (m_faction == 0) {
		for (unsigned i = 0; i < state.enemies.size(); i++) {
			if (util::hasCollided(state.enemies[i]->getPos(), state.enemies[i]->getSize(), state.enemies[i]->getDir(), m_pos, m_size, m_dir)) {
				state.enemies[i]->hit(m_damage, state);
				m_hp = 0;
				break;
			}
		}
	}
}
