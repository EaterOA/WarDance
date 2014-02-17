#include "Actors.hpp"
#include "GameMechanics.hpp"

Grunt::Grunt(sf::Vector2f pos)
    : Fighter(Actor::Image("grunt"), util::ShapeVector(util::Rectangle, 50.f, 50.f), pos, 200, 1)
{
    m_max_v = 50.f;
    m_move_cd = 0;
    m_attack_cd = 0;
    m_radius_close = 30.f;
    m_radius_far = 70.f;
}

void Grunt::cooldown(GameState& state)
{
    Fighter::cooldown(state);
    m_move_cd -= state.elapsed.asSeconds();
    if (m_move_cd < -5) m_move_cd = 0;
}

void Grunt::attack(GameState& state)
{
    m_attack_cd = RAND(500, 2000) / 1000.f;
    shoot(state, REGULAR, 30.f, 7.f);
}

void Grunt::hit(GameState &state, int damage)
{
    if (isDead(state)) return;
    m_hp -= damage;
    if (isDead(state)) {
        state.score += 500;
        if (rand() % 15 == 0) state.items.push_back(new Medkit(m_pos, 10));
    }
}

void Grunt::act(GameState& state)
{
    cooldown(state);

    if (m_move_cd <= 0) {
        m_move_cd = RAND(300, 800) / 1000.f;
        float dist = util::getDist(m_pos, state.player->getPos());
        sf::Vector2f unit = util::getUnitVector(m_pos, state.player->getPos());
        m_dir = util::toDir(unit.x, unit.y);
        if (dist > m_radius_far)
            m_vel = sf::Vector2f(unit.x * m_max_v, unit.y * m_max_v);
        else if (dist < m_radius_close)
            m_vel = sf::Vector2f(-unit.x * m_max_v, -unit.y * m_max_v);
        else
            m_vel = sf::Vector2f(0, 0);
    }
    m_pos += m_vel * state.elapsed.asSeconds();
    
    if (m_attack_cd <= 0) {
        attack(state);
    }
}
