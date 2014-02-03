#include "Actors.hpp"
#include "GameMechanics.hpp"

RegularBullet::RegularBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile("reg_bullet", pos, util::ShapeVector(util::Rectangle, 10, 2), 1, 50, faction)
{
    m_dir = dir;
    float maxV = 100.f;
    if (faction == 0) {
        m_frame = "reg_bullet_p";
        maxV = 140.f;
    }
    m_vel.x = maxV * cos(m_dir);
    m_vel.y = maxV * sin(m_dir);
}

void RegularBullet::act(GameState &state)
{
    Actor::act(state);
    attack(state);
}
