#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include "GameMechanics.hpp"
#include "GameGraphics.hpp"

enum AppState;

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
    unsigned translateOption(AppState state, float x, float y);

    GameMechanics* mAgent;
    GameGraphics* gAgent;

    //Main Menu
    void selectMainChoice(unsigned choice);
    void processMainChoice();
    unsigned m_main_numChoices;
    unsigned m_main_choice;
    sf::Vertex* m_main_blinker, *m_mainMenu;
    int m_main_blinkerAlphaChg;
    sf::Vector2f m_main_blinkerBaseLoc;
    sf::Sprite m_main_bg;
    std::vector<sf::Vertex> m_main;
    std::vector<sf::Vertex> m_mainLit, m_mainDim;
    sf::Text m_mainInfo;
    
    //Pause Menu
    void selectPauseChoice(unsigned choice);
    void processPauseChoice();
    unsigned m_pause_numChoices;
    unsigned m_pause_choice;
    sf::Vertex* m_pauseMenu;
    std::vector<sf::Vertex> m_pause;
    std::vector<sf::Vertex> m_pauseLit, m_pauseDim;

    //Settings Menu
    void selectSettingsChoice(unsigned choice);
    void processSettingsChoice();
    void processSettingsSwitches();
    unsigned m_settings_numChoices;
    unsigned m_settings_choice;
    sf::Sprite m_settings_bg;
    sf::Vertex* m_settingsMenu, *m_settingsSwitches;
    std::vector<sf::Vertex> m_settings;
    std::vector<sf::Vertex> m_settingsLit, m_settingsDim;
    std::vector<sf::Vertex> m_settingsOn, m_settingsOff;

    //HUD
    void updateHUD(const GameState& state);
    sf::Text m_score;
    sf::Vertex *m_hpBar, *m_shieldBar, *m_levelDisplay;
    std::vector<std::vector<sf::Vertex> > m_levelIcons;
    sf::Vertex m_grenade[4];
    std::vector<sf::Vertex> m_grenadeDisplay;
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

    //Select level
    void selectSelectChoice(unsigned choice);
    void processSelectChoice();
    sf::Vertex* m_selectButtons;
    std::vector<sf::Vertex> m_select;
    std::vector<sf::Vertex> m_selectLit, m_selectDim;
    unsigned m_select_choice;
    float m_select_upLitTime, m_select_downLitTime;
};

#endif
