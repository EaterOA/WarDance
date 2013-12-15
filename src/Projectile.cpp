#include "Actors.hpp"
#include "GameMechanics.hpp"

Projectile::Projectile(int type, sf::Vector2f pos, util::ShapeVector size, int hp, int damage, int faction)
	: Fighter(type, size, pos, hp, faction)
{
	m_damage = damage;
	m_faction = faction;
}

bool Projectile::isDead(GameState &state)
{
	return m_hp <= 0 || !state.inMap(m_pos);
}

void Projectile::attack(GameState &state)
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