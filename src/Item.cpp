#include "Actors.hpp"

Item::Item(sf::Vector2f pos, util::ShapeVector size, float dur)
    : Actor(pos, size)
{
    m_duration = dur;
}

float Item::getDuration() const
{
    return m_duration;
}

bool Item::isDead(const BattleState &state) const
{
    return m_duration <= 0;
}
