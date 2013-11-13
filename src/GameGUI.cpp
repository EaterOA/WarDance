#include "GameGUI.hpp"
#include "GameMechanics.hpp"
#include "App.hpp"
#include "Util.hpp"
#include <sstream>

bool GameGUI::init()
{
	if (!m_displayBarTex.loadFromFile("images/hud.png"))
		return 0;
	if (!m_guisheet.loadFromFile("images/guisheet.png"))
		return 0;
	if (!m_regFont.loadFromFile("fonts/stenc_ex.ttf"))
		return 0;
	m_guisheet.setSmooth(true);
	m_displayBar.setTexture(m_displayBarTex);
	m_displayBar.setPosition(0.0f, 600.0f - m_displayBarTex.getSize().y);
	m_hpCase.setPosition(30.f, 480.f);
	m_hpCase.setTexture(m_guisheet);
	m_hpCase.setTextureRect(sf::IntRect(0, 0, 350, 100));
	m_hpBar.setPosition(37.f, 480.f);
	m_hpBar.setTexture(m_guisheet);
	m_hpBar.setTextureRect(sf::IntRect(0, 100, 350, 100));
	m_score.setFont(m_regFont);
	m_score.setPosition(50, 600-50);
	return 1;
}

void GameGUI::updateDisplay(const GameState& state)
{
	float hpPerc = float(state.player->getHP()) / state.player->getMaxHP();
	m_hpBar.setScale(sf::Vector2f(hpPerc, 1.f));
	std::wstringstream wss;
	wss << "Score: " << state.score;
	m_score.setString(wss.str());
}

void GameGUI::transitionState(AppState state)
{
}

void GameGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_displayBar);
	target.draw(m_hpCase);
	target.draw(m_hpBar);
	target.draw(m_score);
}