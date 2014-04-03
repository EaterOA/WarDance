#include "Actors.hpp"
#include "../BattleMechanics.hpp"

Wiper::Wiper(sf::Vector2f pos, int faction)
    : Projectile(pos, util::ShapeVector(util::Stroke, -20, 0), 1, 99999, faction)
{
    m_expandSpeed = 800.f;

    m_image.frame = "wiper";
    m_image.rotated = false;
    if (faction == 0) m_image.color = sf::Color(150, 150, 255);
    else m_image.color = sf::Color(255, 150, 150);
}

bool Wiper::isDead(const BattleState &state) const
{
    return m_size.x > MAX(state.map.x, state.map.y);
}

void Wiper::act(BattleState &state)
{
    float expand = m_expandSpeed * state.elapsed.asSeconds();
    m_size.x += expand;
    m_size.y += expand;
    for (unsigned i = 0; i < state.projectiles.size(); i++) {
        if (state.projectiles[i]->getFaction() == m_faction) continue;
        if (util::hasCollided(m_pos, m_size, m_dir, state.projectiles[i]->getPos(), state.projectiles[i]->getSize(), state.projectiles[i]->getDir())) {
            state.projectiles[i]->hit(state, m_damage);
        }
    }
}
