#include "Actors.hpp"
#include "GameMechanics.hpp"

Wiper::Wiper(sf::Vector2f pos, int faction)
	: Projectile("wiper", pos, util::ShapeVector(util::Stroke, -20, 0), 5, 5, faction)
{
    if (faction == 0) {
        m_frame = "wiper_p";
    }
	m_damage = 99999;
	m_expandSpeed = 800.f;
}

void Wiper::act(GameState &state)
{
	float expand = m_expandSpeed * state.elapsed.asSeconds();
	m_size.x += expand;
    m_size.y += expand;
	for (unsigned i = 0; i < state.projectiles.size(); i++) {
		if (state.projectiles[i]->getFaction() == m_faction) continue;
		if (util::hasCollided(m_pos, m_size, m_dir, state.projectiles[i]->getPos(), state.projectiles[i]->getSize(), state.projectiles[i]->getDir())) {
			state.projectiles[i]->hit(state, m_damage);
		}
	}
	if (m_size.x > MAX(state.map_height, state.map_width)) m_hp = 0;
}
