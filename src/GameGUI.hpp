#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include "Util.hpp"
#include <SFML/Graphics.hpp>
#include "App.hpp"

struct GameState;

class GameGUI : public sf::Drawable, public sf::Transformable
{
public:
	bool init();
	void transitionState();
	void updateGameState(const GameState& state);
	void updateAppState(const std::vector<sf::Event> &keyEvents);
private:

	//Pause Menu
	void selectPauseChoice(unsigned choice);
	void processPauseChoice(unsigned choice);
	unsigned m_pauseMenu_numChoices;
	unsigned m_pauseMenu_choice;

	//Settings Menu
	void selectSettingsChoice(unsigned choice);
	void processSettingsChoice(unsigned choice);
	void processSettingsSwitches();
	unsigned m_settingsMenu_numChoices;
	unsigned m_settingsMenu_choice;

	//GUI Graphics
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void affixTexture(sf::Vertex sprite[4], float* coord);
	void affixPos(sf::Vertex sprite[4], float* coord, float* pos);
    void setAlpha(sf::Vertex sprite[4], unsigned char alpha);
	sf::Font m_regFont;
	sf::Text m_score;
	sf::Sprite m_displayBar;
	sf::Texture m_displayBarTex;
	sf::Sprite m_settings;
	sf::Texture m_settingsTex;
	sf::Vertex* m_hpBar;
	sf::VertexArray m_hudElements;
	sf::VertexArray m_pauseMenu;
	sf::VertexArray m_settingsMenu;
	sf::Texture m_guisheet;
	sf::Texture m_settingsheet;
};

#endif
