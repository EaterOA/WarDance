#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"

Sprinkler::Sprinkler(sf::Vector2f pos): Fighter(3, sf::Vector2f(50.f, 50.f), pos, 10, 1)
{
	m_dir = 0;
}

void Sprinkler::hit(int damage, GameState& state)
{
	if (isDead(state)) return;
	m_hp -= damage;
	if (isDead(state)) state.score += 700;
}

void Sprinkler::attack(GameState& state)
{
	m_attack_cd = 10;
	m_dir += 10.f / 180.f * util::PI;
	if (m_dir >= 2*util::PI) m_dir -= 2*util::PI;

	shoot(state, 20.f, 0);
}

void Sprinkler::act(GameState& state)
{
	Actor::act(state);
	cooldown();
	if (m_attack_cd <= 0) attack(state);
}
