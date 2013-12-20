#include "Actors.hpp"
#include "GameMechanics.hpp"
#include <math.h>

Player::Player(sf::Vector2f pos, int hp)
	: Fighter(2, util::ShapeVector(util::Rectangle, 25.f, 30.f), pos, hp, 0)
{
	m_max_v = sf::Vector2f(150.f, 150.f);
}

void Player::act(GameState &state)
{
	m_vel.x = m_vel.y = 0;
	if (state.W && !state.S) m_vel.y = -m_max_v.y;
	else if (!state.W && state.S) m_vel.y = m_max_v.y;
	if (state.A && !state.D) m_vel.x = -m_max_v.x;
	else if (!state.A && state.D) m_vel.x = m_max_v.x;
	Actor::act(state);
	m_dir = util::toDir(state.mouse.x - m_pos.x, state.mouse.y - m_pos.y);

	cooldown(state);
	if (state.mouseLeft && m_attack_cd <= 0) attack(state);
}

void Player::attack(GameState& state)
{
	m_attack_cd = 150;
	shoot(state, 40.f, 10.f);
}
