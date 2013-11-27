#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"
#include <math.h>

RegularBullet::RegularBullet(sf::Vector2f pos, float dir, int faction): Projectile(5, pos, 3, 5, 5, faction)
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
	return m_hp <= 0 || !util::inMap(m_pos, state);
}

void RegularBullet::attack(GameState& state)
{
	sf::Vector2f hSize(state.player->getSize().x/2, state.player->getSize().y/2);
	if (util::hasCollided(sf::FloatRect(state.player->getPos() - hSize, state.player->getSize()), m_pos, m_radius)) {
		state.player->hit(m_damage, state);
		hit(m_damage, state);
	}
	else if (m_faction == 0) {
		for (unsigned i = 0; i < state.enemies.size(); i++) {
			hSize = sf::Vector2f(state.enemies[i]->getSize().x/2, state.enemies[i]->getSize().y/2);
			if (util::hasCollided(sf::FloatRect(state.enemies[i]->getPos() - hSize, state.enemies[i]->getSize()), m_pos, m_radius)) {
				state.enemies[i]->hit(m_damage, state);
				hit(m_damage, state);
				break;
			}
		}
	}
}
