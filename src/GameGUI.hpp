#ifndef GAMEGUI_HPP
#define GAMEGUI_HPP

#include "GameMechanics.hpp"
#include "GameGraphics.hpp"

class GameGUI : public sf::Drawable, public sf::Transformable
{
public:
    bool init(GameMechanics* m, GameGraphics* g);
    void updateGameState(const GameState& state);
    void updateAppState(const sf::Time& elapsed);
    void transitionAppState();
    void processInput(const std::vector<sf::Event> &events);
    void startLevelEndSequence(const std::map<std::string, int> levelEndStats);
    float getLevelEndSequenceBGFade() const;
    bool isLevelEndSequenceStarted() const;
    bool isLevelEndSequenceDone() const;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    unsigned translateOption(float x, float y);

    GameMechanics* mAgent;
    GameGraphics* gAgent;

    //HUD
    void updateHUD(const GameState& state);
    sf::Text m_score;
    sf::Vertex *m_hpBar, *m_shieldBar, *m_levelDisplay;
    std::vector<std::vector<sf::Vertex> > m_levelIcons;
    sf::Vertex m_grenadeBase[4];
    std::vector<sf::Vertex> m_grenadeDisplay;
    std::map<Player::StatusType, std::vector<sf::Vertex> > m_statusIcons;
    std::vector<sf::Vertex> m_statusDisplay;
    sf::VertexArray m_hud;

    //Level end sequence
    void updateLevelEndSequence(const GameState& state);
    void forwardLevelEndSequence();
    sf::Sprite m_scoringScreen;
    std::vector<sf::Text> m_scoring_numbers;
    sf::Vertex m_scoringLevelDisplay[4];
    sf::Vertex m_nextLevelDisplay[4];
    float m_levelEndSequence_timing;
    int m_levelEndSequence_timing_stage;
    float m_scoring_timing;
    int m_scoring_timing_stage;
};

#endif
