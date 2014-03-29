/*
--------------------------------------------------------------------------------
GameLayer
- Adds layer-management functions specific to WarDance
- Defines WarDance-specific layer types
--------------------------------------------------------------------------------
*/

#ifndef GAMELAYER_HPP
#define GAMELAYER_HPP

#include "AppLayer.hpp"

class GameLayer: public AppLayer
{
public:  
    enum Type {
        SPLASH, MAIN, SELECTLEVEL, SETTINGS, GAME, PAUSE, LEVELENDSEQUENCE, NOFOCUS, CLOSE, NONE
    };
    Type getType() const;
protected:
    Type m_type;
};

extern std::vector<GameLayer*> layer;

namespace Layer {
    GameLayer::Type getTopLayer();
    void back();
    void unfocus();
    void refocus();
    void pauseGame();
    void goToMain();
};


//@@@@@@@@@@@@@@@@@@@@ Layers @@@@@@@@@@@@@@@@@@@@


class NoFocus: public GameLayer
{
public:
    virtual bool init();
    virtual Status tick(std::vector<sf::Event> &e, const sf::Time &t);
    virtual Status draw(sf::RenderWindow &w);
};

class MainMenu: public GameLayer
{
public:
    virtual bool init();
    virtual Status tick(std::vector<sf::Event> &e, const sf::Time &t);
    virtual Status draw(sf::RenderWindow &w);
private:
    void selectChoice(unsigned choice);
    void processChoice();
    unsigned translateOption(float x, float y);

    unsigned m_numChoices;
    unsigned m_choice;
    sf::Vertex* m_blinker, *m_menu;
    int m_blinkerAlphaChg;
    sf::Vector2f m_blinkerBaseLoc;
    sf::Sprite m_bg;
    std::vector<sf::Vertex> m_main;
    std::vector<sf::Vertex> m_mainLit, m_mainDim;
    sf::Text m_info;
};

class SelectLevelDialog: public GameLayer
{
public:
    virtual bool init();
    virtual Status tick(std::vector<sf::Event> &e, const sf::Time &t);
    virtual Status draw(sf::RenderWindow &w);
private:
    void selectChoice(unsigned choice);
    void processChoice();
    unsigned translateOption(float x, float y);

    sf::Vertex* m_buttons;
    std::vector<sf::Vertex> m_select;
    std::vector<sf::Vertex> m_selectLit, m_selectDim;
    unsigned m_choice;
    float m_upLitTime, m_downLitTime;
};

class SettingsMenu: public GameLayer
{
public:
    virtual bool init();
    virtual Status tick(std::vector<sf::Event> &e, const sf::Time &t);
    virtual Status draw(sf::RenderWindow &w);
private:
    void selectChoice(unsigned choice);
    void processChoice();
    void processSwitches();
    unsigned translateOption(float x, float y);

    unsigned m_numChoices;
    unsigned m_choice;
    sf::Sprite m_bg;
    sf::Vertex* m_menu, *m_switches;
    std::vector<sf::Vertex> m_settings;
    std::vector<sf::Vertex> m_settingsLit, m_settingsDim;
    std::vector<sf::Vertex> m_settingsOn, m_settingsOff;
};

class PauseMenu: public GameLayer
{
public:
    virtual bool init();
    virtual Status tick(std::vector<sf::Event> &e, const sf::Time &t);
    virtual Status draw(sf::RenderWindow &w);
private:
    void selectChoice(unsigned choice);
    void processChoice();
    unsigned translateOption(float x, float y);

    unsigned m_numChoices;
    unsigned m_choice;
    sf::Vertex* m_menu;
    std::vector<sf::Vertex> m_pause;
    std::vector<sf::Vertex> m_pauseLit, m_pauseDim;
};


#endif