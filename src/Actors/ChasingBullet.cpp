#include "Actors.hpp"
#include "../BattleMechanics.hpp"

ChasingBullet::ChasingBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(pos, util::ShapeVector(util::Rectangle, 10, 5), 1, 75, faction)
{
    m_dir = dir;
    float maxV = (faction ? 100.f : 140.f);
    m_vel.x = maxV * cos(m_dir);
    m_vel.y = maxV * sin(m_dir);

    m_image.frame = "chase_bullet";
    if (faction == 0) m_image.color = sf::Color(204, 204, 255);
    else m_image.color = sf::Color(255, 172, 48);
}

void ChasingBullet::act(BattleState &state)
{
    Actor::act(state);
    attack(state);
}
