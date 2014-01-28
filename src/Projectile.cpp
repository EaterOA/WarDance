#include "Actors.hpp"
#include "GameMechanics.hpp"

Projectile::Projectile(std::string frame, sf::Vector2f pos, util::ShapeVector size, int hp, int damage, int faction)
    : Fighter(frame, size, pos, hp, faction)
{
    m_damage = damage;
    m_faction = faction;
}

bool Projectile::isDead(const GameState &state) const
{
    //No hp or out of map
    return m_hp <= 0 ||
           m_pos.x + m_size.x < 0 ||
           m_pos.x - m_size.x > state.map_width ||
           m_pos.y + m_size.y < 0 ||
           m_pos.y - m_size.y > state.map_height;
}

void Projectile::attack(GameState &state)
{
    if (m_faction != 0) {
        if (util::hasCollided(state.player->getPos(), state.player->getSize(), state.player->getDir(), m_pos, m_size, m_dir)) {
            state.player->hit(state, m_damage);
            m_hp = 0;
        }
    }
    else {
        for (unsigned i = 0; i < state.enemies.size(); i++) {
            if (util::hasCollided(state.enemies[i]->getPos(), state.enemies[i]->getSize(), state.enemies[i]->getDir(), m_pos, m_size, m_dir)) {
                state.enemies[i]->hit(state, m_damage);
                state.hit++;
                m_hp = 0;
                break;
            }
        }
    }
}
