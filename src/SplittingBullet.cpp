#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"
#include <math.h>

SplittingBullet::SplittingBullet(sf::Vector2f pos, float dir, int hp, int faction): Projectile(6, pos, 3, hp, 20, faction)
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
	return m_hp <= 0 || !util::inMap(m_pos, state);
}

void SplittingBullet::attack(GameState& state)
{
	sf::Vector2f hSize(state.player->getSize().x/2, state.player->getSize().y/2);
	if (util::hasCollided(sf::FloatRect(state.player->getPos() - hSize, state.player->getSize()), m_pos, m_radius)) {
		state.player->hit(m_damage, state);
		m_hp = 0;
	}
	else if (m_faction == 0) {
		for (unsigned i = 0; i < state.enemies.size(); i++) {
			hSize = sf::Vector2f(state.enemies[i]->getSize().x/2, state.enemies[i]->getSize().y/2);
			if (util::hasCollided(sf::FloatRect(state.enemies[i]->getPos() - hSize, state.enemies[i]->getSize()), m_pos, m_radius)) {
				state.enemies[i]->hit(m_damage, state);
				m_hp = 0;
				break;
			}
		}
	}
}
