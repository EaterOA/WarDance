#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <SFML/Graphics.hpp>

struct GameState;

class GameGUI : public sf::Drawable, public sf::Transformable
{
public:
	bool init();
	void transitionState();
	void updateGameState(const GameState& state);
	void updateAppState(const std::vector<sf::Event> &keyEvents);
private:
	void affixTexture(sf::Vertex sprite[4], float coord[]);
	void affixPos(sf::Vertex sprite[4], float coord[], float pos[]);
	void affixPos(sf::Vertex sprite[4], sf::Vector2f topLeft, sf::Vector2f size);
    void setAlpha(sf::Vertex sprite[4], unsigned char alpha);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Main Menu
	void selectMainChoice(unsigned choice);
	void processMainChoice(unsigned choice);
	void mainBlink();
	unsigned m_main_numChoices;
	unsigned m_main_choice;
	int m_main_blink, m_main_blinkChg;
	sf::Vector2f m_main_blinkLoc, m_main_blinkSize;
	sf::Sprite m_main;
	sf::Texture m_mainTex;
	sf::VertexArray m_mainMenu;
	
	//Pause Menu
	void selectPauseChoice(unsigned choice);
	void processPauseChoice(unsigned choice);
	unsigned m_pause_numChoices;
	unsigned m_pause_choice;
	sf::VertexArray m_pauseMenu;

	//Settings Menu
	void selectSettingsChoice(unsigned choice);
	void processSettingsChoice(unsigned choice);
	void processSettingsSwitches();
	unsigned m_settings_numChoices;
	unsigned m_settings_choice;
	sf::Sprite m_settings;
	sf::Texture m_settingsTex;
	sf::VertexArray m_settingsMenu;

	//HUD
	sf::Font m_regFont;
	sf::Text m_score;
	sf::Sprite m_displayBar;
	sf::Texture m_displayBarTex;
	sf::Vertex* m_hpBar;
	sf::VertexArray m_hudElements;
	sf::Texture m_guisheet;
};

#endif
