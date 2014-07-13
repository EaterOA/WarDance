#include "Actors.hpp"
#include "../BattleMechanics.hpp"

ChasingBullet::ChasingBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(pos, util::ShapeVector(util::Rectangle, 10, 5), 1, 75, faction)
{
    m_dir = dir;
    m_maxVel = 130.f;
    m_vel = m_maxVel * sf::Vector2f(cos(m_dir), sin(m_dir));
    m_target = 0;
    m_turnRate = 70.f;
    m_dur = 10.f;

    m_image.frame = "chase_bullet";
}

bool ChasingBullet::isDead(const BattleState &state) const
{
    return m_dur <= 0 || m_hp <= 0;
}

void ChasingBullet::act(BattleState &state)
{
    if (!m_target)
        m_target = state.player;

    m_dur -= state.elapsed.asSeconds();

    sf::Vector2f pVector = m_target->getPos() - m_pos;
    float pDir = util::toDir(pVector.x, pVector.y);
    float ccwdiff = util::diffRad(m_dir, pDir);
    float cwdiff = 2*util::PI - ccwdiff;
    float turnAngleRad = util::toRad(m_turnRate * state.elapsed.asSeconds());
    if (ccwdiff < turnAngleRad || cwdiff < turnAngleRad)
        m_dir = pDir;
    else if (ccwdiff < cwdiff)
        m_dir = util::rotateRad(m_dir, turnAngleRad);
    else
        m_dir = util::rotateRad(m_dir, -turnAngleRad);
    m_vel = m_maxVel * util::toVector(m_dir);

    Actor::act(state);
    attack(state);
}
