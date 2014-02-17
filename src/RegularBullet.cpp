#include "Actors.hpp"
#include "GameMechanics.hpp"

RegularBullet::RegularBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(ActorImage("reg_bullet"), pos, util::ShapeVector(util::Rectangle, 10, 2), 1, 50, faction)
{
    m_dir = dir;
    float maxV = 100.f;
    m_image.rgba = 0xffac30ff;
    if (faction == 0) {
        maxV = 140.f;
        m_image.rgba = 0xccccffff;
        m_image.frame = "reg_bullet_p";
    }
    m_vel.x = maxV * cos(m_dir);
    m_vel.y = maxV * sin(m_dir);
}

void RegularBullet::act(GameState &state)
{
    Actor::act(state);
    attack(state);
}
