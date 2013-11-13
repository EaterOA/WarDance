#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"

Player::Player(sf::Vector2f pos, int hp): Fighter(0, sf::Vector2f(30.f, 30.f), pos, hp, 0)
{
	m_firing = 0;
	m_max_vx = 150.f;
	m_max_vy = 150.f;
}

void Player::act(GameState &state)
{
	m_vel.x = m_vel.y = 0;
	if (state.W && !state.S) m_vel.y = -m_max_vy;
	else if (!state.W && state.S) m_vel.y = m_max_vy;
	if (state.A && !state.D) m_vel.x = -m_max_vx;
	else if (!state.A && state.D) m_vel.x = m_max_vx;
	Actor::act(state);
	m_dir = std::atan((state.mouse.y - m_pos.y)/(state.mouse.x - m_pos.x));
	if (state.mouse.x < m_pos.x) m_dir += util::PI;
	cooldown();
	if (state.mouseLeft && m_attack_cd <= 0) attack(state);
}

void Player::attack(GameState& state)
{
	m_attack_cd = 10;
	shoot(state, 40.f, 10.f);
}