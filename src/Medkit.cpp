#include "Actors.hpp"
#include "GameMechanics.hpp"

Medkit::Medkit(sf::Vector2f pos, float dur)
    : Item(pos, util::ShapeVector(util::Rectangle, 10, 10), dur)
{
    m_hpRestore = 250;

    m_image.frame = "medkit";
    m_image.rotated = false;
}

void Medkit::act(GameState &state)
{
    if (util::hasCollided(state.player->getPos(), state.player->getSize(), state.player->getDir(), m_pos, m_size, m_dir)) {
        state.player->restoreHP(m_hpRestore);
        m_duration = 0;
    }
    m_duration -= state.elapsed.asSeconds();
}
