#include "Actors.hpp"
#include "../BattleMechanics.hpp"
#include <cassert>

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
    m_frame = 0;
    m_maxFrame = 0;
    m_animationTime = 0;
    m_maxAnimationTime = 0;
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

void Actor::animate(float elapsed)
{
    assert(m_maxFrame > 0);         //Don't call animate if there's nothing to animate
    assert(m_maxAnimationTime > 0); //Each frame must last a positive duration
    assert(!m_frameBase.empty());   //Must have filename frame base

    m_animationTime += elapsed;
    if (m_animationTime >= m_maxAnimationTime) {
        m_animationTime = 0;
        m_frame++;
        if (m_frame >= m_maxFrame)
            m_frame = 0;
        std::stringstream ss;
        ss << m_frameBase << m_frame;
        m_image.frame = ss.str();
    }
}