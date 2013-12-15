#include "Actors.hpp"
#include "GameMechanics.hpp"
#include <math.h>

Grunt::Grunt(sf::Vector2f pos)
	: Fighter(3, util::ShapeVector(util::Rectangle, 50.f, 50.f), pos, 20, 1)
{
	m_max_v = 50.f;
	m_decision_cd = 0;
	m_attack_cd = 0;
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
		float normX = vec.x / vr;
		float normY = vec.y / vr;
		m_dir = util::toDir(normX, normY);
		m_vel = sf::Vector2f(normX * m_max_v, normY * m_max_v);
	}
	
	if (m_attack_cd <= 0 && rand() % 100 < 50) attack(state);
}
