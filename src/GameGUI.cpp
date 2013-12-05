#include "GameGUI.hpp"
#include "GameMechanics.hpp"
#include "App.hpp"
#include <sstream>
#include <fstream>

bool GameGUI::init()
{
	std::ifstream fin;
	float coord[400], pos[400];
	
    fin.open("config/guisheet.txt");
	if (!fin) return 0;
	for (int i = 0; i < 100*4 && fin >> coord[i] >> coord[i+1] >> coord[i+2] >> coord[i+3]; i += 4) fin.ignore(1000, '\n');
    fin.close();
    fin.open("config/guimap.txt");
	if (!fin) return 0;
	for (int i = 0; i < 100*2 && fin >> pos[i] >> pos[i+1]; i += 2) fin.ignore(1000, '\n');
    fin.close();
	if (!m_displayBarTex.loadFromFile("images/hud.png")) return 0;
	if (!m_guisheet.loadFromFile("images/guisheet.png")) return 0;
	if (!m_regFont.loadFromFile("fonts/stenc_ex.ttf")) return 0;

	m_guisheet.setSmooth(true);
	m_hudElements = sf::VertexArray(sf::Quads, 2*4);
	float *coordPtr = coord, *posPtr = pos;
	for (unsigned i = 0; i < 2; i++, coordPtr += 4, posPtr += 2) {
		affixTexture(&m_hudElements[i*4], coordPtr);
		affixPos(&m_hudElements[i*4], coordPtr, posPtr);
	}
	m_ingameMenu = sf::VertexArray(sf::Quads, 5*4);
	for (unsigned i = 0; i < 5; i++, coordPtr += 4, posPtr += 2) {
		affixTexture(&m_ingameMenu[i*4], coordPtr);
		affixPos(&m_ingameMenu[i*4], coordPtr, posPtr);
	}
	for (unsigned i = 3; i < 5; i += 2) {
		setAlpha(&m_ingameMenu[i*4], 0);
	}
	m_pauseMenu_choice = 1;
	m_hpBar = &m_hudElements[4];
	
	m_displayBar.setTexture(m_displayBarTex);
	m_displayBar.setPosition(0.0f, 600.0f - m_displayBarTex.getSize().y);
	m_score.setFont(m_regFont);
	m_score.setPosition(50, 600-50);
	appState = GAME;

	return 1;
}

void GameGUI::updateGameState(const GameState& state)
{
	float hpPerc = float(state.player->getHP()) / state.player->getMaxHP();
	m_hpBar[1].position.x = m_hpBar[0].position.x + hpPerc * 350;
	m_hpBar[2].position.x = m_hpBar[0].position.x + hpPerc * 350;
	std::wstringstream wss;
	wss << "Score: " << state.score;
	m_score.setString(wss.str());
}

void GameGUI::selectPauseChoice(unsigned choice)
{
	setAlpha(&m_ingameMenu[m_pauseMenu_choice*8-4], 0);
	setAlpha(&m_ingameMenu[m_pauseMenu_choice*8], 255);
	setAlpha(&m_ingameMenu[choice*8-4], 255);
	setAlpha(&m_ingameMenu[choice*8], 0);
	m_pauseMenu_choice = choice;
}

void GameGUI::processPauseChoice(unsigned choice)
{
	switch (choice) {
	case 1:
		resumeGame();
		break;
	case 2:
		endGame();
		break;
    default:
        break;
	}
}

void GameGUI::updateAppState(const std::vector<sf::Event> &keyEvents)
{
	for (unsigned i = 0; i < keyEvents.size(); i++) {
		if (appState == PAUSED) {
			if (keyEvents[i].key.code == sf::Keyboard::S || keyEvents[i].key.code == sf::Keyboard::Down)
				selectPauseChoice(m_pauseMenu_choice % 2 + 1);
			else if (keyEvents[i].key.code == sf::Keyboard::W || keyEvents[i].key.code == sf::Keyboard::Up)
				selectPauseChoice(m_pauseMenu_choice == 1 ? 2 : m_pauseMenu_choice - 1);
			if (keyEvents[i].key.code == sf::Keyboard::Return)
				processPauseChoice(m_pauseMenu_choice);
		}
		if (keyEvents[i].key.code == sf::Keyboard::Escape) {
			if (appState == PAUSED) resumeGame();
			else if (appState == GAME) pauseGame();
		}
	}
}

void GameGUI::transitionState(AppState prevState)
{
	if (appState == PAUSED)
		selectPauseChoice(1);
}

void GameGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_displayBar);
	target.draw(m_score);
	target.draw(m_hudElements, &m_guisheet);
	if (appState == PAUSED) {
		sf::RectangleShape fade;
		fade.setFillColor(sf::Color(0, 0, 0, 100));
		fade.setPosition(0, 0);
		fade.setSize(sf::Vector2f(800.f, 600.f));
		target.draw(fade);
		target.draw(m_ingameMenu, &m_guisheet);
	}
}

void GameGUI::affixTexture(sf::Vertex sprite[4], float* coord)
{
	sprite[0].texCoords = sf::Vector2f(coord[0], coord[1]);
	sprite[1].texCoords = sf::Vector2f(coord[0] + coord[2], coord[1]);
	sprite[2].texCoords = sf::Vector2f(coord[0] + coord[2], coord[1] + coord[3]);
	sprite[3].texCoords = sf::Vector2f(coord[0], coord[1] + coord[3]);
}

void GameGUI::affixPos(sf::Vertex sprite[4], float* coord, float* pos)
{
	sprite[0].position = sf::Vector2f(pos[0], pos[1]);
	sprite[1].position = sf::Vector2f(pos[0] + coord[2], pos[1]);
	sprite[2].position = sf::Vector2f(pos[0] + coord[2], pos[1] + coord[3]);
	sprite[3].position = sf::Vector2f(pos[0], pos[1] + coord[3]);
}

void GameGUI::setAlpha(sf::Vertex sprite[4], unsigned char alpha)
{
	sprite[0].color.a = alpha;
	sprite[1].color.a = alpha;
	sprite[2].color.a = alpha;
	sprite[3].color.a = alpha;
}
