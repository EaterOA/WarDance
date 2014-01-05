#include "Util.hpp"
#include "Config.hpp"
#include "GameGUI.hpp"
#include "GameMechanics.hpp"
#include "App.hpp"

bool GameGUI::init()
{
	//Loading textures and fonts
	if (!m_guisheet.loadFromFile("images/guisheet.png")) return false;
	if (!m_mainTex.loadFromFile("images/main.png")) return false;
	if (!m_settingsTex.loadFromFile("images/settings.png")) return false;
	if (!m_displayBarTex.loadFromFile("images/hud.png")) return false;
	if (!m_stencil.loadFromFile("fonts/stenc_ex.ttf")) return false;
	if (!m_liberation.loadFromFile("fonts/LiberationSerif-Regular.ttf")) return false;

	//Initializing object counts
	m_pause_numChoices = 3;
	m_settings_numChoices = 1;
	m_main_numChoices = 5;
	unsigned num_hud = 2,
		     num_p = m_pause_numChoices*2 + 1,
			 num_s = m_settings_numChoices*4,
			 num_m = m_main_numChoices*2 + 1;

	//Preparing to load gui object data
	//Data format: 4 texture coordinates, 2 position offsets
	float buf[6];
	std::ifstream fin;
	fin.open("config/guidata.txt");
	if (!fin) return 0;

	//Reading HUD objects
	m_hud = sf::VertexArray(sf::Quads, num_hud*4);
	for (unsigned i = 0; i < num_hud && util::readf(fin, 6, buf, true); i++) {
		affixTexture(&m_hud[i*4], buf);
		affixPos(&m_hud[i*4], buf, buf+4);
	}

	//Reading pause menu objects, initializing appearance
	m_pauseMenu = sf::VertexArray(sf::Quads, num_p*4);
	for (unsigned i = 0; i < num_p && util::readf(fin, 6, buf, true); i++) {
		affixTexture(&m_pauseMenu[i*4], buf);
		affixPos(&m_pauseMenu[i*4], buf, buf+4);
	}

	//Reading settings menu objects
	m_settingsMenu = sf::VertexArray(sf::Quads, num_s*4);
	for (unsigned i = 0; i < num_s && util::readf(fin, 6, buf, true); i++) {
		affixTexture(&m_settingsMenu[i*4], buf);
		affixPos(&m_settingsMenu[i*4], buf, buf+4);
	}

	//Reading main menu objects, initializing appearance
	m_mainMenu = sf::VertexArray(sf::Quads, num_m*4);
	for (unsigned i = 0; i < num_m && util::readf(fin, 6, buf, true); i++) {
		affixTexture(&m_mainMenu[i*4], buf);
		affixPos(&m_mainMenu[i*4], buf, buf+4);
	}

	fin.close();

	//Initializing appearance and configuration of objects
	m_main_choice = 1;
	m_settings_choice = 1;
	m_pause_choice = 1;
	for (unsigned i = 1; i < num_p; i += 2)
		setAlpha(&m_pauseMenu[i*4], 0);
	for (unsigned i = 1; i < num_m; i += 2)
		setAlpha(&m_mainMenu[i*4], 0);
	m_main_blink = 0;
	m_main_blinkChg = 5;
	m_main_blinkLoc = m_mainMenu[0].position;
	m_main_blinkSize = m_mainMenu[2].position - m_main_blinkLoc;
	m_main.setTexture(m_mainTex);
	m_mainInfo.setFont(m_liberation);
	m_mainInfo.setPosition(25, 525);
	m_mainInfo.setCharacterSize(25);
	m_mainInfo.setColor(sf::Color(30, 16, 8));
	m_settings.setTexture(m_settingsTex);
	m_displayBar.setTexture(m_displayBarTex);
	m_displayBar.setPosition(0.0f, (float)APP_HEIGHT - m_displayBarTex.getSize().y);
	m_score.setFont(m_stencil);
	m_score.setPosition(50, 550);
	m_hpBar = &m_hud[4];

	return 1;
}

void GameGUI::affixTexture(sf::Vertex sprite[4], float coord[])
{
	sprite[0].texCoords = sf::Vector2f(coord[0], coord[1]);
	sprite[1].texCoords = sf::Vector2f(coord[0] + coord[2], coord[1]);
	sprite[2].texCoords = sf::Vector2f(coord[0] + coord[2], coord[1] + coord[3]);
	sprite[3].texCoords = sf::Vector2f(coord[0], coord[1] + coord[3]);
}

void GameGUI::affixPos(sf::Vertex sprite[4], float coord[], float pos[])
{
	sprite[0].position = sf::Vector2f(pos[0] - coord[2] / 2.f, pos[1] - coord[3] / 2.f);
	sprite[1].position = sf::Vector2f(pos[0] + coord[2] / 2.f, pos[1] - coord[3] / 2.f);
	sprite[2].position = sf::Vector2f(pos[0] + coord[2] / 2.f, pos[1] + coord[3] / 2.f);
	sprite[3].position = sf::Vector2f(pos[0] - coord[2] / 2.f, pos[1] + coord[3] / 2.f);
}

void GameGUI::affixPos(sf::Vertex sprite[4], sf::Vector2f topLeft, sf::Vector2f size)
{
	sprite[0].position = topLeft;
	sprite[1].position = topLeft + sf::Vector2f(size.x, 0);
	sprite[2].position = topLeft + size;
	sprite[3].position = topLeft + sf::Vector2f(0, size.y);
}

void GameGUI::setAlpha(sf::Vertex sprite[4], unsigned char alpha)
{
	sprite[0].color.a = alpha;
	sprite[1].color.a = alpha;
	sprite[2].color.a = alpha;
	sprite[3].color.a = alpha;
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
	if (choice == 0) choice += m_pause_numChoices;
	else if (choice > m_pause_numChoices) choice -= m_pause_numChoices;
	setAlpha(&m_pauseMenu[m_pause_choice*8-4], 0);
	setAlpha(&m_pauseMenu[m_pause_choice*8], 255);
	setAlpha(&m_pauseMenu[choice*8-4], 255);
	setAlpha(&m_pauseMenu[choice*8], 0);
	m_pause_choice = choice;
}

void GameGUI::processPauseChoice()
{
	if (m_pause_choice == 1) resumeGame();
	else if (m_pause_choice == 2) goToSettings();
	else if (m_pause_choice == 3) goToMain();
}

void GameGUI::selectSettingsChoice(unsigned choice)
{
	if (choice == 0) choice += m_settings_numChoices;
	else if (choice > m_settings_numChoices) choice -= m_settings_numChoices;
	setAlpha(&m_settingsMenu[m_settings_choice*16-8], 0);
	setAlpha(&m_settingsMenu[m_settings_choice*16-4], 255);
	setAlpha(&m_settingsMenu[choice*16-8], 255);
	setAlpha(&m_settingsMenu[choice*16-4], 0);
	m_settings_choice = choice;
}

void GameGUI::processSettingsChoice()
{
	if (m_settings_choice == 1) {
		config["hitbox_enabled"] = !config["hitbox_enabled"];
	}
	processSettingsSwitches();
}

void GameGUI::processSettingsSwitches()
{
	setAlpha(&m_settingsMenu[0], config["hitbox_enabled"] ? 0 : 255);
	setAlpha(&m_settingsMenu[4], config["hitbox_enabled"] ? 255 : 0);
}

void GameGUI::selectMainChoice(unsigned choice)
{
	if (choice == 0) choice += m_main_numChoices;
	else if (choice > m_main_numChoices) choice -= m_main_numChoices;
	setAlpha(&m_mainMenu[m_main_choice*8-4], 0);
	setAlpha(&m_mainMenu[m_main_choice*8], 255);
	setAlpha(&m_mainMenu[choice*8-4], 255);
	setAlpha(&m_mainMenu[choice*8], 0);
	affixPos(&m_mainMenu[0], m_main_blinkLoc + sf::Vector2f(0, (choice-1) * 50.f), m_main_blinkSize);
	m_main_choice = choice;
}

void GameGUI::processMainChoice()
{
	if (m_main_choice == 1) startGame();
    else if (m_main_choice == 3) goToSettings();
	else if (m_main_choice == 5) endGame();
}

void GameGUI::mainBlink()
{
	if (m_main_blink + m_main_blinkChg < 0 ||
		m_main_blink + m_main_blinkChg > 255) m_main_blinkChg *= -1;
	m_main_blink += m_main_blinkChg;
	setAlpha(&m_mainMenu[0], m_main_blink);
}

void GameGUI::updateAppState(const std::vector<sf::Event> &keyEvents)
{
	if (appState == MAIN) {
		mainBlink();
		std::wstringstream wss;
		wss << "Level " << config["level"] << "\nHighscore " << config["highscore"];
		m_mainInfo.setString(wss.str());
	}

	for (unsigned i = 0; i < keyEvents.size(); i++) {
		if (appState == PAUSED) {
			if (conf::pressing(conf::K_DOWN, keyEvents[i].key.code))
				selectPauseChoice(m_pause_choice + 1);
			else if (conf::pressing(conf::K_UP, keyEvents[i].key.code))
				selectPauseChoice(m_pause_choice - 1);
			if (keyEvents[i].key.code == sf::Keyboard::Return)
				processPauseChoice();
		}
		else if (appState == SETTINGS) {
			if (conf::pressing(conf::K_DOWN, keyEvents[i].key.code))
				selectSettingsChoice(m_settings_choice + 1);
			else if (conf::pressing(conf::K_UP, keyEvents[i].key.code))
				selectSettingsChoice(m_settings_choice - 1);
			if (keyEvents[i].key.code == sf::Keyboard::Return)
				processSettingsChoice();
		}
		else if (appState == MAIN) {
			if (conf::pressing(conf::K_DOWN, keyEvents[i].key.code))
				selectMainChoice(m_main_choice + 1);
			else if (conf::pressing(conf::K_UP, keyEvents[i].key.code))
				selectMainChoice(m_main_choice - 1);
			if (keyEvents[i].key.code == sf::Keyboard::Return)
				processMainChoice();
		}
		if (keyEvents[i].key.code == sf::Keyboard::Escape) {
			if (appState == PAUSED) resumeGame();
			else if (appState == GAME) pauseGame();
			else if (appState == SETTINGS) {
				if (prevState == PAUSED) pauseGame();
				else if (prevState == MAIN) goToMain();
			}
		}
	}
}

void GameGUI::transitionState()
{
	if (appState == PAUSED) {
		selectPauseChoice(1);
	}
	if (appState == SETTINGS) {
		selectSettingsChoice(1);
		processSettingsSwitches();
	}
	if (appState == MAIN) {
		selectMainChoice(1);
	}
}

void GameGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (appState == MAIN) {
		target.draw(m_main);
		target.draw(m_mainMenu, &m_guisheet);
		target.draw(m_mainInfo);
	}
	else if (appState == GAME || appState == PAUSED) {
		target.draw(m_displayBar);
		target.draw(m_score);
		target.draw(m_hud, &m_guisheet);
		if (appState == PAUSED) {
			sf::RectangleShape fade;
			fade.setFillColor(sf::Color(0, 0, 0, 100));
			fade.setPosition(0, 0);
			fade.setSize(sf::Vector2f((float)APP_WIDTH, (float)APP_HEIGHT));
			target.draw(fade);
			target.draw(m_pauseMenu, &m_guisheet);
		}
	}
	else if (appState == SETTINGS) {
		target.draw(m_settings);
		target.draw(m_settingsMenu, &m_guisheet);
	}
}
