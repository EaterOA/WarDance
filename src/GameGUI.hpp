#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <SFML/Graphics.hpp>

struct GameState;

class GameGUI : public sf::Drawable, public sf::Transformable
{
public:
	bool init();
	void transitionState(const GameState& state);
	void updateGameState(const GameState& state);
	void updateAppState(const std::vector<sf::Event> &keyEvents);
private:
	void copySprite(sf::Vertex src[4], sf::Vertex dest[4]);
	void affixTexture(sf::Vertex sprite[4], float coord[]);
	void affixPos(sf::Vertex sprite[4], float coord[], float pos[]);
	void affixPos(sf::Vertex sprite[4], sf::Vector2f topLeft, sf::Vector2f size);
    void setAlpha(sf::Vertex sprite[4], unsigned char alpha);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

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

    //Level end
    void initLevelEnd(const GameState& state);
    void updateLevelEnd(const GameState& state);
    void forwardLevelEnd();
    float m_levelEnd_timing;
    int m_levelEnd_timing_stage;
    int m_scoring_amt[5];
    float m_scoring_timing;
    int m_scoring_timing_stage;
};

#endif
