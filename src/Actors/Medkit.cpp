#include "Actors.hpp"
#include "../BattleMechanics.hpp"

Medkit::Medkit(sf::Vector2f pos)
    : Item(pos, util::ShapeVector(util::Rectangle, 10, 10), 10)
{
    m_restore = 250;

    m_image.frame = "medkit";
    m_image.rotated = false;
}

void Medkit::act(BattleState &state)
{
    if (util::hasCollided(state.player->getPos(), state.player->getSize(), state.player->getDir(), m_pos, m_size, m_dir)) {
        state.player->restoreHP(m_restore);
        m_dur = 0;
    }
    m_dur -= state.elapsed.asSeconds();
}
