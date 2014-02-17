#include "Actors.hpp"
#include "GameMechanics.hpp"

Alien::Alien(sf::Vector2f pos)
    : Fighter(ActorImage("alien"), util::ShapeVector(util::Rectangle, 60.f, 45.f), pos, 2500, 1)
{
    m_dir = 0;
    m_move_cd = 0;
    m_gunDir1 = m_gunDir2 = 0;
    m_max_v = 30.f;
}

void Alien::hit(GameState& state, int damage)
{
    if (isDead(state)) return;
    m_hp -= damage;
    if (isDead(state)) state.score += 4000;
}

void Alien::cooldown(GameState& state)
{
    Fighter::cooldown(state);
    m_move_cd -= state.elapsed.asSeconds();
    if (m_move_cd < -5) m_move_cd = 0;
}

void Alien::attack(GameState& state)
{
    m_attack_cd = 0.1f;
    m_gunDir1 = util::rotateRad(m_gunDir1, util::toRad(17.f));
    m_gunDir2 = util::rotateRad(m_gunDir2, util::toRad(11.f));
    sf::Vector2f gunPos1 = util::translatePos(m_pos, m_dir, 18.f, 25.f);
    sf::Vector2f gunPos2 = util::translatePos(m_pos, m_dir, 18.f, -25.f);
    shoot(state, REGULAR, gunPos1, m_dir + m_gunDir1);
    shoot(state, REGULAR, gunPos2, m_dir + m_gunDir2);
}

void Alien::act(GameState& state)
{
    Actor::act(state);
    cooldown(state);
    if (m_move_cd <= 0) {
        m_move_cd = RAND(200, 600) / 1000.f;
        sf::Vector2f unit = util::getUnitVector(m_pos, state.player->getPos());
        m_dir = util::toDir(unit.x, unit.y);
        m_vel = sf::Vector2f(unit.x * m_max_v, unit.y * m_max_v);
    }
    if (m_attack_cd <= 0) {
        attack(state);
    }
}
