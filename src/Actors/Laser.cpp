#include "Actors.hpp"
#include "../BattleMechanics.hpp"

Laser::Laser(sf::Vector2f pos, float dir, int faction)
    : Projectile(pos, util::ShapeVector(util::Rectangle, 600, 12), 7, 800, faction)
{
    m_dir = dir;
    m_vel.x = 0;
    m_vel.y = 0;
    m_pos.x += m_size.x/2 * cos(m_dir);
    m_pos.y += m_size.x/2 * sin(m_dir);
    m_dur = 0.15f;
    m_fadeDur = 0.08f;
    m_countHit = false;

    m_image.frame = "m_laser";
    m_image.resized = true;
    m_image.size.y = 9;
    m_image.size.x = 600;
    if (m_faction == 0) m_image.color = sf::Color(92, 242, 253);
    else m_image.color = sf::Color(230, 138, 0);
}

void Laser::attack(BattleState &state)
{
    int tickDmg = (int)(m_damage * state.elapsed.asSeconds());
    if (m_faction != 0) {
        if (util::hasCollided(state.player->getPos(), state.player->getSize(), state.player->getDir(), m_pos, m_size, m_dir)) {
            state.player->hit(state, tickDmg);
            state.shot += tickDmg;
        }
    }
    else {
        for (unsigned i = 0; i < state.enemies.size(); i++) {
            if (util::hasCollided(state.enemies[i]->getPos(), state.enemies[i]->getSize(), state.enemies[i]->getDir(), m_pos, m_size, m_dir)) {
                state.enemies[i]->hit(state, tickDmg);
                if (!m_countHit) {
                    state.hit++;
                    m_countHit = 1;
                }
            }
        }
    }
}

void Laser::act(BattleState& state)
{
    if (m_dur >= 0) {
        attack(state);
        m_dur -= state.elapsed.asSeconds();
        m_fadeDur += state.elapsed.asSeconds();
        if (m_fadeDur > 0.1f) m_fadeDur = 0.1f;
    }
    else {
        m_fadeDur -= state.elapsed.asSeconds();
        if (m_fadeDur <= 0)
            m_hp = 0;
    }
    m_image.color.a = (unsigned char)(255 * (m_fadeDur / 0.1f));
}

