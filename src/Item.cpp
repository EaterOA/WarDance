#include "Actors.hpp"

Item::Item(const ActorImage& img, sf::Vector2f pos, util::ShapeVector size, float dur)
    : Actor(img, pos, size)
{
    m_duration = dur;
}

float Item::getDuration() const
{
    return m_duration;
}

bool Item::isDead(const GameState &state) const
{
    return m_duration <= 0;
}
