#include "Actors.hpp"
#include "BattleMechanics.hpp"

Grenade::Grenade(sf::Vector2f pos, sf::Vector2f dest)
    : Projectile(pos, util::ShapeVector(util::Circle, 90), 1, 1000, 0)
{
    sf::Vector2f v = dest - pos;
    m_dir = util::toDir(v.x, v.y);
    m_vel.x = 250.f * cos(m_dir);
    m_vel.y = 250.f * sin(m_dir);
    m_time = sqrt(util::dot(v, v)) / 250.f;
    m_animationTime = 0.499f;
    m_attacked = false;

    m_image.frame = "grenade";
}

bool Grenade::isDead(const BattleState &state) const
{
    return m_animationTime <= 0;
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
    if (m_time <= 0) {
        if (!m_attacked) {
            attack(state);
            m_attacked = true;
        }
        else if (m_animationTime > 0) {
            std::stringstream ss;
            ss << "exp" << (int)(m_animationTime * 24.f);
            m_image.frame = ss.str();
        }
        m_animationTime -= state.elapsed.asSeconds();
    }
    else {
        Actor::act(state);
        m_dir = util::rotateRad(m_dir, state.elapsed.asSeconds() * 12);
        m_time -= state.elapsed.asSeconds();
    }
}
