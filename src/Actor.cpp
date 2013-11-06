#include "Actors.hpp"
#include "GameMechanics.hpp"

Actor::Actor(int type, sf::Vector2f pos)
{
	m_type = type;
	m_pos = pos;
}

void Actor::act(GameState& state)
{
	m_vel += m_acc * state.elapsed.asSeconds();
	if (abs(m_vel.x) > 200) m_vel.x = (float)(m_vel.x < 0 ? -200 : 200);
	if (abs(m_vel.y) > 200) m_vel.y = (float)(m_vel.y < 0 ? -200 : 200);
	m_pos += m_vel * state.elapsed.asSeconds();
}

int Actor::getType() const
{
	return m_type;
}

sf::Vector2f Actor::getPos() const
{
	return m_pos;
}

float Actor::getDir() const
{
	return m_dir;
}