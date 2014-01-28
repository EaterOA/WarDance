#include "Actors.hpp"
#include "GameMechanics.hpp"

GrenadeWiper::GrenadeWiper(sf::Vector2f pos)
    : Wiper(pos, 0)
{
    m_frame = "gwiper_p";
    m_size.x = -50;
    m_expandSpeed = 1200.f;
    m_expandAccel = -1000.f;
    m_expandJerk = -10000.f;
    m_range = 300.f;
}

bool GrenadeWiper::isDead(const GameState& state) const
{
    return m_size.y >= m_range;
}

void GrenadeWiper::attack(GameState &state)
{
    //Min damage: 100, max damage: 350
    m_damage = (int)(350 - 250 * m_size.y / m_range);
    for (unsigned i = 0; i < state.enemies.size(); i++) {
        Fighter* e = state.enemies[i];
        if (m_hit.find(e) != m_hit.end()) continue;
        if (util::hasCollided(m_pos, m_size, m_dir, e->getPos(), e->getSize(), e->getDir())) {
            e->hit(state, m_damage);
            m_hit.insert(e);
        }
    }
}

void GrenadeWiper::act(GameState &state)
{
    attack(state);
    m_expandSpeed += m_expandAccel * state.elapsed.asSeconds();
    m_expandAccel += m_expandJerk * state.elapsed.asSeconds();
    if (m_expandSpeed <= 200) m_expandSpeed = 200.f;
    Wiper::act(state);
}
