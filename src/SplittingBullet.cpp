#include "Actors.hpp"
#include "GameMechanics.hpp"

SplittingBullet::SplittingBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(Actor::Image("split_bullet"), pos, util::ShapeVector(util::Circle, 3.f), rand() % 40 + 15, 75, faction)
{
    m_dir = dir;
    m_vel.x = 100.f * cos(m_dir);
    m_vel.y = 100.f * sin(m_dir);
}

void SplittingBullet::act(GameState &state)
{
    if (--m_hp <= 0) {
        for (float deg = 0; deg < 360; deg += 10) {
            state.projectiles.push_back(new RegularBullet(m_pos, util::toRad(deg), m_faction));
        }
    }
    else {
        Actor::act(state);
        attack(state);
    }
}
