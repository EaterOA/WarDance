#include "Actors.hpp"
#include "GameMechanics.hpp"

Laser::Laser(sf::Vector2f pos, float dir, int faction)
	: Projectile("laser", pos, util::ShapeVector(util::Rectangle, 750, 15), 15, 1, faction)
{
	m_dir = dir;
	m_vel.x = 0;
	m_vel.y = 0;
	m_time = 0.15f;
}

Laser::Laser(sf::Vector2f pos, float dir, float unitX, float unitY, int faction)
	: Projectile("laser", pos, util::ShapeVector(util::Rectangle, 750, 15), 15, 1, faction)
{
	m_dir = dir;
	m_vel.x = 0;
	m_vel.y = 0;
	m_time = 0.15f;
}

void Laser::attack(GameState &state)
{
	if (m_faction != 0) {
		if (util::hasCollided(state.player->getPos(), state.player->getSize(), state.player->getDir(), m_pos, m_size, m_dir)) {
			state.player->hit(state, m_damage);
		}
	}
	else {
		for (unsigned i = 0; i < state.enemies.size(); i++) {
			if (util::hasCollided(state.enemies[i]->getPos(), state.enemies[i]->getSize(), state.enemies[i]->getDir(), m_pos, m_size, m_dir)) {
				state.enemies[i]->hit(state, m_damage);
			}
		}
	}
}

void Laser::act(GameState& state)
{
	if(m_time <= 0)
		m_hp = 0;

	attack(state);
	m_time -= state.elapsed.asSeconds();
}
