#ifndef BATTLEHUD_HPP
#define BATTLEHUD_HPP

#include "BattleMechanics.hpp"

class BattleHUD : public sf::Drawable, public sf::Transformable
{
public:
    bool init();
    void updateState(const BattleState& state);
    void setTransition(float alpha);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Text m_score;
    sf::Vertex *m_hpBar, *m_shieldBar, *m_levelDisplay;
    std::vector<std::vector<sf::Vertex> > m_levelIcons;
    sf::Vertex m_grenadeBase[4];
    std::vector<sf::Vertex> m_grenadeDisplay;
    std::map<Player::StatusType, std::vector<sf::Vertex> > m_statusIcons;
    std::vector<sf::Vertex> m_statusDisplay;
    sf::VertexArray m_hud;
    sf::Vertex m_nextLevelDisplay[4];
};

#endif
