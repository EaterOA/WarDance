#include "Actors.hpp"
#include "GameMechanics.hpp"

Sprinkler::Sprinkler(sf::Vector2f pos)
	: Fighter("sprinkler", util::ShapeVector(util::Circle, 20.f), pos, 100, 1)
{
	m_attack_cd = 0;
}

void Sprinkler::hit(GameState &state, int damage)
{
	if (isDead(state)) return;
	m_hp -= damage;
	if (isDead(state)) state.score += 700;
}

void Sprinkler::attack(GameState& state)
{
	m_attack_cd = 0.15f;
	m_dir = util::rotateRad(m_dir, util::toRad(10.f));
	shoot(state, REGULAR, 20.f, 0);
}

void Sprinkler::act(GameState& state)
{
	Actor::act(state);
	cooldown(state);
	if (m_attack_cd <= 0) attack(state);
}
