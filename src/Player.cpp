#include "Actors.hpp"
#include "GameMechanics.hpp"

Player::Player(sf::Vector2f pos)
	: Fighter("player", util::ShapeVector(util::Rectangle, 25.f, 30.f), pos, 50, 0)
{
	m_max_v = sf::Vector2f(150.f, 150.f);
}

void Player::act(GameState &state)
{
	//Compute velocity based on player activity
	m_vel.x = m_vel.y = 0;
	if (state.W && !state.S) m_vel.y = -m_max_v.y;
	else if (!state.W && state.S) m_vel.y = m_max_v.y;
	if (state.A && !state.D) m_vel.x = -m_max_v.x;
	else if (!state.A && state.D) m_vel.x = m_max_v.x;
	m_pos += m_vel * state.elapsed.asSeconds();

	//Bound by map
	if (m_pos.x < 0) m_pos.x = 0;
	else if (m_pos.x > (float)state.map_width) m_pos.x = (float)state.map_width;
	if (m_pos.y < 0) m_pos.y = 0;
	else if (m_pos.y > (float)state.map_height) m_pos.y = (float)state.map_height;
	m_dir = util::toDir(state.mouse.x - m_pos.x, state.mouse.y - m_pos.y);

	cooldown(state);
	if (state.mouseLeft && m_attack_cd <= 0) attack(state);
}

void Player::attack(GameState& state)
{
	m_attack_cd = 150;
	shoot(state, REGULAR, 40.f, 10.f);
}

void Player::restoreHP(int amt)
{
    m_hp += amt;
    if (m_hp > m_maxHp) m_hp = m_maxHp;
}
