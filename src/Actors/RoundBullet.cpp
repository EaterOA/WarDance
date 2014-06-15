#include "Actors.hpp"
#include "../BattleMechanics.hpp"

RoundBullet::RoundBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(pos, util::ShapeVector(util::Circle, 11), 1, 50, faction)
{
    m_dir = dir;
    float velMax = 75.f;
    m_vel.x = velMax * cos(m_dir);
    m_vel.y = velMax * sin(m_dir);

    m_image.frame = "round_bullet";
    m_image.rotated = false;
}

void RoundBullet::act(BattleState &state)
{
    Actor::act(state);
    attack(state);
}
