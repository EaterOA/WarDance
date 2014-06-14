#include "Actors.hpp"
#include "../BattleMechanics.hpp"

SplittingBulletRound::SplittingBulletRound(sf::Vector2f pos, float dir, int faction)
    : Projectile(pos, util::ShapeVector(util::Circle, 19.f), 1, 100, faction)
{
    m_dir = dir;
    m_vel.x = 100.f * cos(m_dir);
    m_vel.y = 100.f * sin(m_dir);
    m_maxDur = RAND(30, 70) / 10.f;
    m_dur = m_maxDur;
    
    m_frameCounter = 0;
    m_frames = 3;
    m_animationTime = 0;
    m_maxAnimationTime = 0.05f;

    m_image.frame = "split_bullet_rnd0";
    m_image.rotated = false;
}

void SplittingBulletRound::act(BattleState &state)
{
    m_dur -= state.elapsed.asSeconds();
    m_image.color.b = m_image.color.g = (unsigned char)(55 + 200 * (m_dur / m_maxDur));

    m_animationTime += state.elapsed.asSeconds();
    if (m_animationTime >= m_maxAnimationTime) {
        m_animationTime = 0;
        m_frameCounter++;
        if (m_frameCounter >= m_frames)
            m_frameCounter = 0;
        std::stringstream ss;
        ss << "split_bullet_rnd" << m_frameCounter;
        m_image.frame = ss.str();
    }

    if (m_dur <= 0) {
        for (float deg = 0; deg < 360; deg += 10) {
            state.projectiles.push_back(new RoundBullet(m_pos, util::toRad(deg), m_faction));
        }
        m_hp = 0;
    }
    else {
        Actor::act(state);
        attack(state);
    }
}
