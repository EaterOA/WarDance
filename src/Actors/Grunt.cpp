#include "Actors.hpp"
#include "../BattleMechanics.hpp"

Grunt::Grunt(sf::Vector2f pos, Item::Type drop)
    : Enemy(util::ShapeVector(util::Rectangle, 50.f, 50.f), pos, 200, 1, 500, drop)
{
    m_velMax = 50.f;
    m_turnCD = 0;
    m_attackCD = 0;
    m_radiusClose = 30.f;
    m_radiusFar = 70.f;

    m_image.frame = "grunt";
}


Grunt::Grunt(util::ShapeVector size, sf::Vector2f pos, int hp, int faction, int bounty, Item::Type drop)
    : Enemy(size, pos, hp, faction, bounty, drop)
{
}

void Grunt::cooldown(BattleState& state)
{
    Fighter::cooldown(state);
    m_turnCD -= state.elapsed.asSeconds();
}

void Grunt::attack(BattleState& state)
{
    m_attackCD = RAND(500, 2000) / 1000.f;
    shoot(state, REGULAR, 30.f, 7.f);
}

void Grunt::hit(BattleState &state, int damage)
{
    if (isDead(state)) return;
    m_hp -= damage;
    if (isDead(state)) onDeath(state);
}

void Grunt::act(BattleState& state)
{
    cooldown(state);

    if (m_turnCD <= 0) {
        m_turnCD = RAND(300, 800) / 1000.f;
        sf::Vector2f unit = util::getUnitVector(m_pos, state.player->getPos());
        m_dir = util::toDir(unit.x, unit.y);
        float dist = util::getDist(m_pos, state.player->getPos());
        if (dist > m_radiusFar)
            m_vel = m_velMax * sf::Vector2f(unit.x, unit.y);
        else if (dist < m_radiusClose)
            m_vel = m_velMax * sf::Vector2f(-unit.x, -unit.y);
        else
            m_vel = sf::Vector2f(0, 0);
    }
    sf::Vector2f nextPos = m_pos + m_vel * state.elapsed.asSeconds();
    if (util::inside(m_pos, state.map) && !util::inside(nextPos, state.map))
        m_vel = sf::Vector2f(0, 0);
    m_pos += m_vel * state.elapsed.asSeconds();
    
    if (m_attackCD <= 0)
        attack(state);
}
