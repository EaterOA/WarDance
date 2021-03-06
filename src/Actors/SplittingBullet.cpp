#include "Actors.hpp"
#include "../BattleMechanics.hpp"

SplittingBullet::SplittingBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(pos, util::ShapeVector(util::Circle, 10.f), 1, 75, faction)
{
    m_dir = dir;
    m_vel.x = 100.f * cos(m_dir);
    m_vel.y = 100.f * sin(m_dir);
    m_durMax = RAND(30, 70) / 10.f;
    m_dur = m_durMax;

    m_image.frame = "split_bullet";
}

void SplittingBullet::act(BattleState &state)
{
    m_dur -= state.elapsed.asSeconds();
    m_avel = util::PI * 8.f * (1 - m_dur / m_durMax);
    m_image.color.b = m_image.color.g = (unsigned char)(55 + 200 * (m_dur / m_durMax));
    m_dir += m_avel * state.elapsed.asSeconds();

    if (m_dur <= 0) {
        for (float deg = 0; deg < 360; deg += 10)
            state.projectiles.push_back(new RegularBullet(m_pos, util::toRad(deg), m_faction));
        m_hp = 0;
    }
    else {
        Actor::act(state);
        attack(state);
    }
}
