#include "Actors.hpp"
#include "GameMechanics.hpp"

Sprinkler::Sprinkler(sf::Vector2f pos, Item::Type drop)
    : Enemy(util::ShapeVector(util::Circle, 20.f), pos, 100, 1, 700, drop)
{
    m_attack_cd = 0;

    m_image.frame = "sprinkler";
}

void Sprinkler::hit(GameState &state, int damage)
{
    if (isDead(state)) return;
    m_hp -= damage;
    if (isDead(state)) onDeath(state);
}

void Sprinkler::attack(GameState& state)
{
    m_attack_cd = 0.15f;
    m_dir = util::rotateRad(m_dir, util::toRad(10.f));
    shoot(state, REGULAR, 20.f, 0);
}

void Sprinkler::act(GameState& state)
{
    Actor::act(state);
    cooldown(state);
    if (m_attack_cd <= 0) attack(state);
}
