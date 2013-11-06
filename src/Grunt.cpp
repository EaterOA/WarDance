#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"

Grunt::Grunt(sf::Vector2f pos): Fighter(2, sf::Vector2f(50.f, 50.f), pos, 20, 1)
{
	m_decision_cd = 0;
	m_max_v = sf::Vector2f(50.f, 50.f);
}

void Grunt::attack(GameState& state)
{
	m_attack_cd = 50;
	shootTowards(state, state.player->getPos(), 40.f, 7.f);
}

void Grunt::act(GameState& state)
{
	Actor::act(state);
	cooldown();
	if (m_decision_cd-- > 0) return;
	m_decision_cd = 10;
	if (std::rand() % 100 < 50) {
		sf::Vector2f vec = state.player->getPos() - m_pos;
		float vr = Util::getHyp(vec);
		float ny = vec.y / vr;
		float nx = vec.x / vr;
		m_dir = std::atan(vec.y / vec.x) * 180 / 3.1415f;
		if (vec.x < 0) m_dir += 180;
		m_vel = sf::Vector2f(nx * 50.f, ny * 50.f);	
	}
	else if (m_attack_cd <= 0) {
		attack(state);
	}
}