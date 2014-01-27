#include "Actors.hpp"
#include "GameMechanics.hpp"

GrenadeWiper::GrenadeWiper(sf::Vector2f pos)
	: Wiper(pos, 0)
{
	m_frame = "gwiper_p";
	m_size.x = -50;
	m_expandSpeed = 1200.f;
	m_expandAccel = -1000.f;
	m_expandJerk = -10000.f;
	m_damage = 1800;
}

bool GrenadeWiper::isDead(const GameState& state) const
{
	return m_expandSpeed <= 0;
}

void GrenadeWiper::attack(GameState &state)
{
    int tickDmg = (int)(m_damage * state.elapsed.asSeconds());
	for (unsigned i = 0; i < state.enemies.size(); i++) {
		if (util::hasCollided(m_pos, m_size, m_dir, state.enemies[i]->getPos(), state.enemies[i]->getSize(), state.enemies[i]->getDir())) {
			state.enemies[i]->hit(state, tickDmg);
		}
	}
	m_damage -= (int)(3600*state.elapsed.asSeconds());
}

void GrenadeWiper::act(GameState &state)
{
	Wiper::act(state);
	attack(state);
	m_expandSpeed += m_expandAccel * state.elapsed.asSeconds();
	m_expandAccel += m_expandJerk * state.elapsed.asSeconds();
}