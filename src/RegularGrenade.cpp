#include "Actors.hpp"
#include "GameMechanics.hpp"

RegularGrenade::RegularGrenade(sf::Vector2f pos, sf::Vector2f dest)
	: Projectile("split_bullet", pos, util::ShapeVector(util::Circle, 100), 5, 5, 0)
{
	m_damage = 100;
	sf::Vector2f v = dest - pos;
	m_dir = util::toDir(v.x, v.y);
	m_vel.x = 150.f * cos(m_dir);
	m_vel.y = 150.f * sin(m_dir);
	m_time = sqrt(util::dot(v, v)) / 150.f;
}

void RegularGrenade::attack(GameState &state)
{
	for (unsigned i = 0; i < state.enemies.size(); i++) {
		if (util::hasCollided(m_pos, m_size.x, state.enemies[i]->getPos(), 0)) {
			state.enemies[i]->hit(state, m_damage);
		}
	}
	for (unsigned i = 0; i < state.projectiles.size(); i++) {
		if (util::hasCollided(m_pos, m_size.x, state.projectiles[i]->getPos(), 0)) {
			state.projectiles[i]->hit(state, m_damage);
		}
	}
	m_hp = 0;
}

void RegularGrenade::act(GameState &state)
{
	if (m_time <= 0) {
		attack(state);
	}
	else {
		Actor::act(state);
		m_time -= state.elapsed.asSeconds();
	}
}