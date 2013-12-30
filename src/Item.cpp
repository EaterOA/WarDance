#include "Actors.hpp"

Item::Item(std::string frame, sf::Vector2f pos, util::ShapeVector size, float dur)
    : Actor(frame, pos, size)
{
    m_duration = dur;
}

float Item::getDuration() const
{
    return m_duration;
}

bool Item::isDead(GameState &state) const
{
    return m_duration <= 0;
}
