#include "Actors.hpp"
#include "GameMechanics.hpp"

Actor::Actor(std::string frame, sf::Vector2f pos, util::ShapeVector size)
{
	m_frame = frame;
	m_pos = pos;
    m_size = size;
	m_dir = 0;
}

void Actor::act(GameState& state)
{
	m_vel += m_acc * state.elapsed.asSeconds();
	m_pos += m_vel * state.elapsed.asSeconds();
}

const std::string& Actor::getFrame() const
{
	return m_frame;
}

sf::Vector2f Actor::getPos() const
{
	return m_pos;
}

float Actor::getDir() const
{
	return m_dir;
}

util::ShapeVector Actor::getSize() const
{
    return m_size;
}
