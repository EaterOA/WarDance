#include "Actors.hpp"
#include "GameMechanics.hpp"

RoundBullet::RoundBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(ActorImage("round_bullet"), pos, util::ShapeVector(util::Circle, 11), 1, 50, faction)
{
    m_dir = dir;
    float maxV = 75.f;
    m_vel.x = maxV * cos(m_dir);
    m_vel.y = maxV * sin(m_dir);
}

void RoundBullet::act(GameState &state)
{
    Actor::act(state);
    attack(state);
}
