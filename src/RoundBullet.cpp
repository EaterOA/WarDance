#include "Actors.hpp"
#include "GameMechanics.hpp"

RoundBullet::RoundBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(pos, util::ShapeVector(util::Circle, 11), 1, 50, faction)
{
    m_dir = dir;
    float maxV = 75.f;
    m_vel.x = maxV * cos(m_dir);
    m_vel.y = maxV * sin(m_dir);

    m_image.frame = "round_bullet";
    m_image.rotated = false;
}

void RoundBullet::act(GameState &state)
{
    Actor::act(state);
    attack(state);
}
