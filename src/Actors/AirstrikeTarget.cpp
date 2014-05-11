#include "Actors.hpp"
#include "../BattleMechanics.hpp"

AirstrikeTarget::AirstrikeTarget(sf::Vector2f pos)
    : Enemy(util::ShapeVector(util::Point), pos, 1, 1)
{
    m_attack_cd = 0;
    m_collidable = false;
    m_summoned = false;

    m_image.frame = "airstrike_target";
}

void AirstrikeTarget::act(BattleState& state)
{
    m_countdown -= state.elapsed.asSeconds();
    
    if (m_countdown > 0) {
    }
    else {
        if (!m_summoned) {
            m_summoned = true;
        }
        m_countdown = 0;
    }
}
