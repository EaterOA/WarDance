#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"

RegularBullet::RegularBullet(sf::Vector2f pos, int faction): Projectile(1, pos, 3, 5, faction)
{
	m_attack_cd = 0;
}

void RegularBullet::act(GameState &state)
{
	Actor::act(state);
	attack(state);
}

bool RegularBullet::isDead()
{
	return m_hp <= 0 || !Util::inMap(m_pos);
}

void RegularBullet::attack(GameState& state)
{
	sf::Vector2f hSize(state.player->getSize().x/2, state.player->getSize().y/2);
	if (Util::hasCollided(sf::FloatRect(state.player->getPos() - hSize, state.player->getSize()), m_pos, m_radius)) {
		state.player->hit(m_hp);
		hit(m_hp);
	}
	else if (m_faction == 0) {
		for (unsigned i = 0; i < state.enemies.size(); i++) {
			hSize = sf::Vector2f(state.enemies[i]->getSize().x/2, state.enemies[i]->getSize().y/2);
			if (Util::hasCollided(sf::FloatRect(state.enemies[i]->getPos() - hSize, state.enemies[i]->getSize()), m_pos, m_radius)) {
				state.enemies[i]->hit(m_hp);
				hit(m_hp);
				break;
			}
		}
	}
}