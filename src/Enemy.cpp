#include "Actors.hpp"
#include "BattleMechanics.hpp"

Enemy::Enemy(util::ShapeVector size, sf::Vector2f pos, int hp, int faction, int bounty, Item::Type drop)
    : Fighter(size, pos, hp, faction)
{
    m_bounty = bounty;
    m_drop = drop;
}

void Enemy::onDeath(BattleState& state)
{
    state.score += m_bounty;
    if (m_drop == Item::Medkit) {
        state.items.push_back(new Medkit(m_pos));
    }
}
