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
	void selectMainChoice(int choice);
	void processMainChoice();
	void mainBlink();
	int m_main_numChoices;
	int m_main_choice;
	int m_main_blink, m_main_blinkChg;
	sf::Vector2f m_main_blinkLoc, m_main_blinkSize;
	sf::Sprite m_main;
	sf::Texture m_mainTex;
	sf::VertexArray m_mainMenu;
	
	//Pause Menu
	void selectPauseChoice(int choice);
	void processPauseChoice();
	int m_pause_numChoices;
	int m_pause_choice;
	sf::VertexArray m_pauseMenu;

	//Settings Menu
	void selectSettingsChoice(int choice);
	void processSettingsChoice();
	void processSettingsSwitches();
	int m_settings_numChoices;
	int m_settings_choice;
	sf::Sprite m_settings;
	sf::Texture m_settingsTex;
	sf::VertexArray m_settingsMenu;

	//HUD
	sf::Font m_regFont;
	sf::Text m_score;
	sf::Sprite m_displayBar;
	sf::Texture m_displayBarTex;
	sf::Vertex* m_hpBar;
	sf::VertexArray m_hud;
	sf::Texture m_guisheet;
};

#endif
