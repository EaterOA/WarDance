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
	if (!m_scoringScreenTex.loadFromFile("images/scoring.png")) return false;
	if (!m_stencil.loadFromFile("fonts/stenc_ex.ttf")) return false;
	if (!m_liberation.loadFromFile("fonts/LiberationSerif-Regular.ttf")) return false;

	//Initializing object counts
	m_pause_numChoices = 3;
	m_settings_numChoices = 1;
	m_main_numChoices = 5;
	unsigned num_level_icons = 10,
		     num_p = m_pause_numChoices*2 + 1,
			 num_s = m_settings_numChoices*4,
			 num_m = m_main_numChoices*2 + 1,
			 num_e = 6;

	//Preparing to load gui object data
	//Data format: 4 texture coordinates, 2 position offsets
	float buf[6];
	std::ifstream fin;
	fin.open("config/guidata.txt");
	if (!fin) return false;

	//Reading HUD objects
	//- Grenades
	if (!util::readf(fin, 6, buf, true)) return false;
	affixTexture(m_grenade, buf);
	affixPos(m_grenade, buf, buf+4);
	//- Bars
	m_hud = sf::VertexArray(sf::Quads, 5*4);
	for (unsigned i = 0; i < 4 && util::readf(fin, 6, buf, true); i++) {
		affixTexture(&m_hud[i*4], buf);
		affixPos(&m_hud[i*4], buf, buf+4);
	}
	//- Level icons
	m_levelIcons = std::vector<std::vector<sf::Vertex> >(num_level_icons, std::vector<sf::Vertex>(4));
	for (unsigned i = 0; i < num_level_icons && util::readf(fin, 6, buf, true); i++) {
		affixTexture(&m_levelIcons[i][0], buf);
		affixPos(&m_levelIcons[i][0], buf, buf+4);
	}
	m_hpBar = &m_hud[1*4];
	m_shieldBar = &m_hud[2*4];
	m_levelDisplay = &m_hud[4*4];
	m_score.setFont(m_stencil);
	m_score.setPosition(22, 560);
	m_score.setCharacterSize(24);

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

	//Reading level end sequence text positions
	m_scoring_numbers = std::vector<sf::Text>(6, sf::Text());
	for (unsigned i = 0; i < num_e && util::readf(fin, 6, buf, true); i++) {
		m_scoring_numbers[i].setPosition(buf[4], buf[5]);
		m_scoring_numbers[i].setFont(m_stencil);
		if (i+1 < num_e) {
			m_scoring_numbers[i].setColor(sf::Color::White);
		}
		else {
			m_scoring_numbers[i].setColor(sf::Color::Green);
			m_scoring_numbers[i].setCharacterSize(40);
		}
	}
	//Hackish way of copying levelIcon size but specifying a different position for scoring_levelIcon
	util::readf(fin, 6, buf, true);
	sf::Vector2f pos(buf[4], buf[5]);
	sf::Vector2f size = m_levelIcons[0][2].position - m_levelIcons[0][0].position;
	affixPos(m_scoring_levelIcon, pos, size);

	fin.close();

	//Initializing appearance and configuration of objects
	m_guisheet.setSmooth(true);
	m_main.setTexture(m_mainTex);
	m_settings.setTexture(m_settingsTex);
	m_scoringScreen.setTexture(m_scoringScreenTex);
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
	m_mainInfo.setFont(m_liberation);
	m_mainInfo.setPosition(25, 525);
	m_mainInfo.setCharacterSize(25);
	m_mainInfo.setColor(sf::Color(30, 16, 8));

	return true;
}

void GameGUI::copySprite(const sf::Vertex src[4], sf::Vertex dest[4])
{
	for (unsigned i = 0; i < 4; i++)
		dest[i] = src[i];
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

void GameGUI::startLevelEndSequence(const std::map<std::string, int> levelEndStats)
{
	std::string dataList[] = {"accuracy", "time", "", "", "", "bonus"};
	for (unsigned i = 0; i < 6; i++) {
		std::wstringstream wss;
		wss << levelEndStats.find(dataList[i])->second;
		m_scoring_numbers[i].setString(wss.str());
	}
	sf::Vector2f pos = m_scoring_levelIcon[0].position;
	sf::Vector2f size = m_scoring_levelIcon[2].position - pos;
	unsigned idx = (unsigned)(config["level"]-1);
	copySprite(&m_levelIcons[idx][0], m_scoring_levelIcon);
	affixPos(m_scoring_levelIcon, pos, size);
    m_scoring_timing_stage = 0;
    m_scoring_timing = 1.f;
    m_levelEndSequence_timing_stage = 0;
    m_levelEndSequence_timing = 2.5f;
}

bool GameGUI::isLevelEndSequenceStarted() const
{
	for (std::vector<AppState>::reverse_iterator iter = appStates.rbegin(); iter != appStates.rend(); iter++)
		if (*iter == LEVELENDSEQUENCE)
			return true;
	return false;
}

bool GameGUI::isLevelEndSequenceDone() const
{
	return m_levelEndSequence_timing_stage == 5;
}

float GameGUI::getLevelEndSequenceBGFade() const
{
	if (isLevelEndSequenceStarted()) {
		if (m_levelEndSequence_timing_stage == 4) {
			if (m_levelEndSequence_timing >= 0) return m_levelEndSequence_timing / 1.5f;
			return 0.f;
		}
	}
	return 1.f;
}

void GameGUI::forwardLevelEndSequence()
{
    if (m_levelEndSequence_timing_stage <= 1) {
        m_levelEndSequence_timing_stage = 1;
		m_scoring_timing_stage = 5;
		m_scoring_timing = 0.f;
    }
    else if (m_levelEndSequence_timing_stage == 2) {
        m_levelEndSequence_timing_stage = 3;
        m_levelEndSequence_timing = 2.f;
    }
}

void GameGUI::updateLevelEndSequence(const GameState& state)
{
	//std::cout << m_levelEndSequence_timing_stage << " " << m_levelEndSequence_timing << "\n";
	//Initial wait for scorescreen to appear
    if (m_levelEndSequence_timing_stage == 0) {
        m_levelEndSequence_timing -= state.elapsed.asSeconds();
        if (m_levelEndSequence_timing <= 0) {
            m_levelEndSequence_timing_stage++;
        }
    }
	//Waiting for all the scores to be displayed
    else if (m_levelEndSequence_timing_stage == 1) {
        m_scoring_timing -= state.elapsed.asSeconds();
        if (m_scoring_timing <= 0) {
            m_scoring_timing_stage++;
            if (m_scoring_timing_stage == 6) {
				getGameState().score += getGameState().level_bonus;
                m_levelEndSequence_timing_stage++;
            }
            else {
                m_scoring_timing = 0.5f;
            }
        }
    }
	//Score screen gone, waiting for background fade to begin
    else if (m_levelEndSequence_timing_stage == 3) {
        m_levelEndSequence_timing -= state.elapsed.asSeconds();
        if (m_levelEndSequence_timing <= 0) {
			m_levelEndSequence_timing_stage++;
			m_levelEndSequence_timing = 1.5f;
        }
    }
	//Background fade
	else if (m_levelEndSequence_timing_stage == 4) {
        m_levelEndSequence_timing -= state.elapsed.asSeconds();
        if (m_levelEndSequence_timing <= 0) {
			m_levelEndSequence_timing_stage++;
        }
	}
}

void GameGUI::updateHUD(const GameState& state)
{
	//Scaling bars
	float hpPerc = float(state.player->getHP()) / state.player->getMaxHP();
	m_hpBar[1].position.x = m_hpBar[0].position.x + hpPerc * 241.4f;
	m_hpBar[2].position.x = m_hpBar[0].position.x + hpPerc * 241.4f;
	float shieldPerc = float(state.player->getShield()) / state.player->getMaxShield();
	m_shieldBar[1].position.x = m_shieldBar[0].position.x + shieldPerc * 132.5f;
	m_shieldBar[2].position.x = m_shieldBar[0].position.x + shieldPerc * 132.5f;
	//Updating score
	std::wstringstream wss;
	wss << state.score;
	m_score.setString(wss.str());
	//Updating level icon
	unsigned idx = (unsigned)(config["level"]-1);
	copySprite(&m_levelIcons[idx][0], m_levelDisplay);
	//Updating grenade display
	m_grenadeDisplay = std::vector<sf::Vertex>();
	for (int i = 0; i < state.player->getNumGrenades(); i++) {
		sf::Vertex grenadeSprite[4];
		copySprite(m_grenade, grenadeSprite);
		for (unsigned j = 0; j < 4; j++)
            grenadeSprite[j].position += sf::Vector2f(-25.f*i, 0);
	    m_grenadeDisplay.insert(m_grenadeDisplay.end(), grenadeSprite, grenadeSprite+4);	
    }
}

void GameGUI::updateGameState(const GameState& state)
{
    if (getAppState() == LEVELENDSEQUENCE) {
        GameGUI::updateLevelEndSequence(state);
    }
    GameGUI::updateHUD(state);
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
	if (m_pause_choice == 1) back();
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

void GameGUI::processInput(const std::vector<sf::Event> &keyEvents)
{
	if (getAppState() == MAIN) {
		mainBlink();
		std::wstringstream wss;
		wss << "Level " << config["level"] << "\nHighscore " << config["highscore"];
		m_mainInfo.setString(wss.str());
	}

	for (unsigned i = 0; i < keyEvents.size(); i++) {
		if (getAppState() == LEVELENDSEQUENCE) {
			if (keyEvents[i].key.code == sf::Keyboard::Return)
				forwardLevelEndSequence();
		}
		else if (getAppState() == PAUSED) {
			if (conf::pressing(conf::K_DOWN, keyEvents[i].key.code))
				selectPauseChoice(m_pause_choice + 1);
			else if (conf::pressing(conf::K_UP, keyEvents[i].key.code))
				selectPauseChoice(m_pause_choice - 1);
			if (keyEvents[i].key.code == sf::Keyboard::Return)
				processPauseChoice();
		}
		else if (getAppState() == SETTINGS) {
			if (conf::pressing(conf::K_DOWN, keyEvents[i].key.code))
				selectSettingsChoice(m_settings_choice + 1);
			else if (conf::pressing(conf::K_UP, keyEvents[i].key.code))
				selectSettingsChoice(m_settings_choice - 1);
			if (keyEvents[i].key.code == sf::Keyboard::Return)
				processSettingsChoice();
		}
		else if (getAppState() == MAIN) {
			if (conf::pressing(conf::K_DOWN, keyEvents[i].key.code))
				selectMainChoice(m_main_choice + 1);
			else if (conf::pressing(conf::K_UP, keyEvents[i].key.code))
				selectMainChoice(m_main_choice - 1);
			if (keyEvents[i].key.code == sf::Keyboard::Return)
				processMainChoice();
		}
		if (keyEvents[i].key.code == sf::Keyboard::Escape) {
			if (getAppState() == GAME || getAppState() == LEVELENDSEQUENCE) pauseGame();
			else if (getAppState() != MAIN) back();
		}
	}
}

void GameGUI::transitionAppState()
{
	if (getAppState() == PAUSED) {
		selectPauseChoice(1);
	}
	if (getAppState() == SETTINGS) {
		selectSettingsChoice(1);
		processSettingsSwitches();
	}
	if (getAppState() == MAIN) {
		selectMainChoice(1);
	}
}

void GameGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (appStates.back() == MAIN) {
		target.draw(m_main);
		target.draw(m_mainMenu, &m_guisheet);
		target.draw(m_mainInfo);
	}
	else if (appStates.back() == SETTINGS) {
		target.draw(m_settings);
		target.draw(m_settingsMenu, &m_guisheet);
	}
	else {
		sf::RenderStates s(&m_guisheet);
		for (unsigned i = 0; i < appStates.size(); i++) {
			if (appStates[i] == GAME) {
				target.draw(m_score);
				target.draw(m_hud, s);
				if (!m_grenadeDisplay.empty()) target.draw(&m_grenadeDisplay[0], m_grenadeDisplay.size(), sf::Quads, s);
			}
			else if (appStates[i] == LEVELENDSEQUENCE) {
				if (m_levelEndSequence_timing_stage == 1 || m_levelEndSequence_timing_stage == 2) {
					target.draw(m_scoringScreen);
					target.draw(m_scoring_levelIcon, 4, sf::Quads, s);
					for (int j = 0; j < m_scoring_timing_stage; j++) {
						target.draw(m_scoring_numbers[(unsigned)j]);
					}
				}
				else if (m_levelEndSequence_timing_stage == 4) {

				}
			}
			else if (appStates[i] == PAUSED) {
				sf::RectangleShape fade;
				fade.setFillColor(sf::Color(0, 0, 0, 100));
				fade.setPosition(0, 0);
				fade.setSize(sf::Vector2f((float)APP_WIDTH, (float)APP_HEIGHT));
				target.draw(fade);
				target.draw(m_pauseMenu, &m_guisheet);
			}
		}
	}
}
