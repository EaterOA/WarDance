#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <SFML/Graphics.hpp>

enum AppState;
struct GameState;

class GameGUI : public sf::Drawable, public sf::Transformable
{
public:
	bool init();
	void transitionState(AppState state);
	void updateGameState(const GameState& state);
	void updatePauseState();
private:
	AppState m_appState;

	//Pause Menu
	void selectPauseChoice(int choice);
	int m_pauseMenu_choice;
	int m_pauseMenu_cd;

	//GUI Graphics
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void affixTexture(sf::Vertex sprite[4], float* coord);
	void affixPos(sf::Vertex sprite[4], float* coord, float* pos);
void GameGUI::setAlpha(sf::Vertex sprite[4], unsigned char alpha);
	sf::Font m_regFont;
	sf::Text m_score;
	sf::Sprite m_displayBar;
	sf::Texture m_displayBarTex;
	sf::Vertex* m_hpBar;
	sf::VertexArray m_hudElements;
	sf::VertexArray m_ingameMenu;
	sf::Texture m_guisheet;
};

#endif