#include "Actors.hpp"
#include "../BattleMechanics.hpp"

Disappoint::Disappoint(sf::Vector2f pos, Item::Type drop)
    : Grunt(util::ShapeVector(util::Circle, 20.f), pos, 300, 1, 800, drop)
{
    m_velMax = 40.f;
    m_turnCD = 0;
    m_attackCD = 0;
    m_radiusClose = 400.f;
    m_radiusFar = 450.f;

    m_image.frame = "disappoint";
}

void Disappoint::attack(BattleState& state)
{
    if (m_attackCD1 <= 0) {
        m_attackCD1 = RAND(800, 2400) / 1000.f;
        sf::Vector2f gunPos = util::translatePos(m_pos, m_dir, -3.f, -10.f);
        sf::Vector2f unit = util::getUnitVector(gunPos, state.player->getPos());
        float gunDir = util::toDir(unit.x, unit.y);
        shoot(state, ROUND, gunPos, gunDir);
    }
    if (m_attackCD2 <= 0) {
        m_attackCD2 = RAND(800, 2400) / 1000.f;
        sf::Vector2f gunPos = util::translatePos(m_pos, m_dir, -3.f, 10.f);
        sf::Vector2f unit = util::getUnitVector(gunPos, state.player->getPos());
        float gunDir = util::toDir(unit.x, unit.y);
        shoot(state, ROUND, gunPos, gunDir);
    }
}

void Disappoint::cooldown(BattleState &state)
{
    Fighter::cooldown(state);
    m_turnCD -= state.elapsed.asSeconds();
    m_attackCD1 -= state.elapsed.asSeconds();
    m_attackCD2 -= state.elapsed.asSeconds();
}


void Disappoint::onDeath(BattleState &state)
{
    Grunt::onDeath(state);
    shoot(state, SPLITTING_RND, 0, 0);
}