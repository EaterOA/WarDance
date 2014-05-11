#include "Actors.hpp"
#include "../BattleMechanics.hpp"

Actor::Image::Image()
{
    frame = "";
    color = sf::Color(255, 255, 255, 255);
    rotated = true;
    resized = false;
}

Actor::Actor(sf::Vector2f pos, util::ShapeVector size)
{
    m_pos = pos;
    m_size = size;
    m_dir = 0;
}

Actor::~Actor()
{
}

void Actor::act(BattleState& state)
{
    m_vel += m_acc * state.elapsed.asSeconds();
    m_pos += m_vel * state.elapsed.asSeconds();
}

const Actor::Image& Actor::getImage() const
{
    return m_image;
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
