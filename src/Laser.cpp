#include "Actors.hpp"
#include "GameMechanics.hpp"

Laser::Laser(sf::Vector2f pos, float dir, int faction)
	: Projectile("m_laser", pos, util::ShapeVector(util::Rectangle, 600, 15), 7, 800, faction)
{
	m_dir = dir;
	m_vel.x = 0;
	m_vel.y = 0;
	m_time = 0.15f;
	m_fadeTime = 0.08f;
	if (m_faction == 0) m_frame = "m_laser_p";
	m_countHit = false;
}

void Laser::attack(GameState &state)
{
    int tickDmg = (int)(m_damage * state.elapsed.asSeconds());
	sf::Vector2f translated_pos(m_pos.x + m_size.x/2 * cos(m_dir), m_pos.y + m_size.x/2 * sin(m_dir));
	if (m_faction != 0) {
		if (util::hasCollided(state.player->getPos(), state.player->getSize(), state.player->getDir(), translated_pos, m_size, m_dir)) {
			state.player->hit(state, tickDmg);
			state.shot += tickDmg;
		}
	}
	else {
		for (unsigned i = 0; i < state.enemies.size(); i++) {
			if (util::hasCollided(state.enemies[i]->getPos(), state.enemies[i]->getSize(), state.enemies[i]->getDir(), translated_pos, m_size, m_dir)) {
				state.enemies[i]->hit(state, tickDmg);
				if (!m_countHit) {
					state.hit++;
					m_countHit = 1;
				}
			}
		}
	}
}

void Laser::act(GameState& state)
{
	if (m_time >= 0) {
		attack(state);
		m_time -= state.elapsed.asSeconds();
		m_fadeTime += state.elapsed.asSeconds();
		if (m_fadeTime > 0.1f) m_fadeTime = 0.1f;
	}
	else {
		m_fadeTime -= state.elapsed.asSeconds();
		if (m_fadeTime <= 0)
			m_hp = 0;
	}
}

float Laser::getFadePerc() const
{
	return m_fadeTime / 0.1f;
}
