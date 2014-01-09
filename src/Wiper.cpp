#include "Actors.hpp"
#include "GameMechanics.hpp"

Wiper::Wiper(sf::Vector2f pos)
	: Projectile("wiper", pos, util::ShapeVector(util::Circle, 0), 5, 5, 0)
{
	m_damage = 99999;
	m_dir = 0;
	m_expandSpeed = 800.f;
}

void Wiper::act(GameState &state)
{
	float expand = m_expandSpeed * state.elapsed.asSeconds();
	m_size.x += expand;
	for (unsigned i = 0; i < state.projectiles.size(); i++) {
		if (state.projectiles[i]->getFaction() == 0) continue;
		if (util::hasCollided(m_pos, m_size.x, state.projectiles[i]->getPos(), 0)) {
			state.projectiles[i]->hit(state, m_damage);
		}
	}
	if (m_size.x > MAX(state.map_height, state.map_width)) m_hp = 0;
}
