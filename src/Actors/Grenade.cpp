#include "Actors.hpp"
#include "../BattleMechanics.hpp"

Grenade::Grenade(sf::Vector2f pos, sf::Vector2f dest)
    : Projectile(pos, util::ShapeVector(util::Circle, 90), 1, 1000, 0)
{
    sf::Vector2f v = dest - pos;
    m_dir = util::toDir(v.x, v.y);
    m_vel.x = 250.f * cos(m_dir);
    m_vel.y = 250.f * sin(m_dir);
    m_dur = sqrt(util::dot(v, v)) / 250.f;
    m_attacked = false;
    m_dead = false;
    
    m_maxFrame = 12;
    m_maxAnimationTime = 0.04f;
    m_frameBase = "exp";
    //Trick to make the first animate() switch to exp0
    m_frame = -1;
    m_animationTime = m_maxAnimationTime;

    m_image.frame = "grenade";
}

bool Grenade::isDead(const BattleState &state) const
{
    return m_dead;
}

void Grenade::attack(BattleState &state)
{
    for (unsigned i = 0; i < state.enemies.size(); i++) {
        if (util::hasCollided(m_pos, m_size.x, state.enemies[i]->getPos(), 0)) {
            state.enemies[i]->hit(state, m_damage);
        }
    }
    state.projectiles.push_back(new GrenadeWiper(m_pos));
}

void Grenade::act(BattleState &state)
{
    if (m_dur <= 0) {
        if (!m_attacked) {
            attack(state);
            m_attacked = true;
        }
        else {
            if (m_animationTime+state.elapsed.asSeconds() >= m_maxAnimationTime && m_frame+1 >= m_maxFrame)
                m_dead = true;
            else
                animate(state.elapsed.asSeconds());
        }
    }
    else {
        Actor::act(state);
        m_dir = util::rotateRad(m_dir, state.elapsed.asSeconds() * 12);
        m_dur -= state.elapsed.asSeconds();
    }
}
