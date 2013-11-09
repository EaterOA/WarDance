#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <SFML/Graphics.hpp>

struct GameState;

class GameGUI : public sf::Drawable, public sf::Transformable
{
public:
	bool init();
	void updateDisplay(const GameState& state);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Font m_regFont;
	sf::Text m_hp;
	sf::Text m_score;
	sf::Sprite m_displayBar;
	sf::Sprite m_hpCase, m_hpBar;
	sf::Texture m_displayBarTex;
	sf::Texture m_guisheet;
};

#endif