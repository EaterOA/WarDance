#include "Actors.hpp"
#include "../BattleMechanics.hpp"

ChasingBullet::ChasingBullet(sf::Vector2f pos, float dir, int faction)
    : Projectile(pos, util::ShapeVector(util::Rectangle, 10, 5), 1, 75, faction)
{
    m_dir = dir;
    m_maxVel = 130.f;
    m_vel = m_maxVel * sf::Vector2f(cos(m_dir), sin(m_dir));
    m_target = 0;
    m_turnDur = 0;
    m_maxTurnDur = 0.1f;
    m_dur = 10.f;

    m_image.frame = "chase_bullet";
}

bool ChasingBullet::isDead(const BattleState &state) const
{
    return m_dur <= 0 || m_hp <= 0;
}

void ChasingBullet::act(BattleState &state)
{
    m_dur -= state.elapsed.asSeconds();
    m_turnDur += state.elapsed.asSeconds();

    if (m_turnDur >= m_maxTurnDur) {
        m_turnDur = 0;
        if (!m_target)
            m_target = state.player;

        sf::Vector2f unit = util::getUnitVector(m_pos, state.player->getPos());
        m_dir = util::toDir(unit.x, unit.y);
        m_vel = m_maxVel * sf::Vector2f(unit.x, unit.y);
    }

    Actor::act(state);
    attack(state);
}
