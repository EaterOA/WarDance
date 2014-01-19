#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include "GameMechanics.hpp"
#include "GameGraphics.hpp"

class GameGUI : public sf::Drawable, public sf::Transformable
{
public:
	bool init(GameMechanics* m, GameGraphics* g);
	void updateGameState(const GameState& state);
	void transitionAppState();
	void processInput(const std::vector<sf::Event> &keyEvents);
    void startLevelEndSequence(const std::map<std::string, int> levelEndStats);
	float getLevelEndSequenceBGFade() const;
	bool isLevelEndSequenceStarted() const;
	bool isLevelEndSequenceDone() const;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	GameMechanics* mAgent;
	GameGraphics* gAgent;

	//Main Menu
	void selectMainChoice(unsigned choice);
	void processMainChoice();
	void mainBlink();
	unsigned m_main_numChoices;
	unsigned m_main_choice;
	int m_main_blink, m_main_blinkChg;
	sf::Vector2f m_main_blinkLoc, m_main_blinkSize;
	sf::Sprite m_main;
	sf::Texture m_mainTex;
	sf::VertexArray m_mainMenu;
	sf::Text m_mainInfo;
	
	//Pause Menu
	void selectPauseChoice(unsigned choice);
	void processPauseChoice();
	unsigned m_pause_numChoices;
	unsigned m_pause_choice;
	sf::VertexArray m_pauseMenu;

	//Settings Menu
	void selectSettingsChoice(unsigned choice);
	void processSettingsChoice();
	void processSettingsSwitches();
	unsigned m_settings_numChoices;
	unsigned m_settings_choice;
	sf::Sprite m_settings;
	sf::Texture m_settingsTex;
	sf::VertexArray m_settingsMenu;

	//HUD
    void updateHUD(const GameState& state);
	sf::Font m_stencil;
	sf::Font m_liberation;
	sf::Text m_score;
	sf::Vertex *m_hpBar, *m_shieldBar, *m_levelDisplay;
	std::vector<std::vector<sf::Vertex> > m_levelIcons;
	sf::Vertex m_grenade[4];
	std::vector<sf::Vertex> m_grenadeDisplay;
	sf::VertexArray m_hud;
	sf::Texture m_guisheet;

    //Level end sequence
    void updateLevelEndSequence(const GameState& state);
    void forwardLevelEndSequence();
	sf::Sprite m_scoringScreen;
	sf::Texture m_scoringScreenTex;
	std::vector<sf::Text> m_scoring_numbers;
	sf::Vertex m_scoring_levelIcon[4];
    float m_levelEndSequence_timing;
    int m_levelEndSequence_timing_stage;
    float m_scoring_timing;
    int m_scoring_timing_stage;
};

#endif
