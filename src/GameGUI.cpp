#include "GameGUI.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"
#include <sstream>

void GameGUI::init() throw (const char*)
{
	if (!m_displayBarTex.loadFromFile("Images/hud.png"))
		throw "loading GUI textures";
	if (!m_regFont.loadFromFile("Fonts/stenc_ex.ttf"))
		throw "loading fonts";
	m_displayBar.setTexture(m_displayBarTex);
	m_displayBar.setPosition(0.0f, 600.0f - m_displayBarTex.getSize().y);
	m_hp.setColor(sf::Color::White);
	m_hp.setFont(m_regFont);
	m_hp.setCharacterSize(30);
	m_hp.setStyle(sf::Text::Bold);
	m_hp.setPosition(100, 600-100);
}

void GameGUI::updateDisplay(const GameState& state)
{
	std::wstringstream wss;
	wss << L"HP: " << state.player->getHP();
	m_hp.setString(wss.str());
	wss.clear();
}

void GameGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_displayBar);
	target.draw(m_hp);
}