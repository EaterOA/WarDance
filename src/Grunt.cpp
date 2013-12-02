#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"
#include <math.h>

Grunt::Grunt(sf::Vector2f pos): Fighter(3, sf::Vector2f(50.f, 50.f), pos, 20, 1)
{
	m_decision_cd = 0;
}

void Grunt::attack(GameState& state)
{
	m_attack_cd = 50;
	shoot(state, 40.f, 7.f);
}

void Grunt::hit(int damage, GameState &state)
{
	if (isDead(state)) return;
	m_hp -= damage;
	if (isDead(state)) state.score += 500;
}

void Grunt::act(GameState& state)
{
	Actor::act(state);
	cooldown();
	if (m_decision_cd-- > 0) return;
	m_decision_cd = 10;
	if (rand() % 100 < 75) {
		sf::Vector2f vec = state.player->getPos() - m_pos;
		float vr = util::getLen(vec);
		float ny = vec.y / vr;
		float nx = vec.x / vr;
		m_dir = atan(vec.y / vec.x);
		if (vec.x < 0) m_dir += util::PI;
		m_vel = sf::Vector2f(nx * 50.f, ny * 50.f);

		if (m_attack_cd <= 0 && rand() % 100 < 50) attack(state);
	}
}
