#include "Actors.hpp"
#include "../BattleMechanics.hpp"

Alien::Alien(sf::Vector2f pos, Item::Type drop)
    : Enemy(util::ShapeVector(util::Rectangle, 60.f, 45.f), pos, 2500, 1, 4000, drop)
{
    m_dir = 0;
    m_turnCD = 0;
    m_gunDir1 = m_gunDir2 = 0;
    m_velMax = 30.f;

    m_image.frame = "alien"; 
}

void Alien::hit(BattleState& state, int damage)
{
    if (isDead(state)) return;
    m_hp -= damage;
    if (isDead(state)) onDeath(state);
}

void Alien::cooldown(BattleState& state)
{
    Fighter::cooldown(state);
    m_turnCD -= state.elapsed.asSeconds();
    if (m_turnCD < -5) m_turnCD = 0;
}

void Alien::attack(BattleState& state)
{
    m_attackCD = 0.1f;
    m_gunDir1 = util::rotateRad(m_gunDir1, util::toRad(17.f));
    m_gunDir2 = util::rotateRad(m_gunDir2, util::toRad(11.f));
    sf::Vector2f gunPos1 = util::translatePos(m_pos, m_dir, 18.f, 25.f);
    sf::Vector2f gunPos2 = util::translatePos(m_pos, m_dir, 18.f, -25.f);
    shoot(state, REGULAR, gunPos1, m_dir + m_gunDir1);
    shoot(state, REGULAR, gunPos2, m_dir + m_gunDir2);
}

void Alien::act(BattleState& state)
{
    Actor::act(state);
    cooldown(state);
    if (m_turnCD <= 0) {
        m_turnCD = RAND(200, 600) / 1000.f;
        sf::Vector2f unit = util::getUnitVector(m_pos, state.player->getPos());
        m_dir = util::toDir(unit.x, unit.y);
        m_vel = m_velMax * sf::Vector2f(unit.x, unit.y);
    }
    if (m_attackCD <= 0) {
        attack(state);
    }
}
