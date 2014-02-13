#include "Util.hpp"
#include "Config.hpp"
#include "GameGUI.hpp"
#include "App.hpp"

bool GameGUI::init(GameMechanics* m, GameGraphics* g)
{
    mAgent = m;
    gAgent = g;

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
             num_l = 4,
             num_e = 6;

    //Preparing to load gui object data
    //Data format: 2 base texture coordinates, 2 texture size, 2 position coordinates
    sf::Vector2f texCoord, size, pos;
    std::ifstream fin;
    fin.open("config/guidata.txt");
    if (!fin) return false;

    //Reading HUD objects
    //- Grenades
    if (!util::read3v2f(fin, texCoord, size, pos, true)) return false;
    util::affixTexture(m_grenade, texCoord, size);
    util::affixPos(m_grenade, pos, size, 0);
    //- Bars
    m_hud = sf::VertexArray(sf::Quads, 5*4);
    for (unsigned i = 0; i < 4 && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_hud[i*4], texCoord, size);
        util::affixPos(&m_hud[i*4], pos, size, 0);
    }
    //- Level icons
    m_levelIcons = std::vector<std::vector<sf::Vertex> >(num_level_icons, std::vector<sf::Vertex>(4));
    for (unsigned i = 0; i < num_level_icons && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_levelIcons[i][0], texCoord, size);
        util::affixPos(&m_levelIcons[i][0], pos, size, 0);
    }
    m_hpBar = &m_hud[1*4];
    m_shieldBar = &m_hud[2*4];
    m_levelDisplay = &m_hud[4*4];
    m_score.setFont(m_stencil);
    m_score.setPosition(22, 560);
    m_score.setCharacterSize(24);

    //Reading pause menu objects, initializing appearance
    m_pauseMenu = sf::VertexArray(sf::Quads, num_p*4);
    for (unsigned i = 0; i < num_p && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_pauseMenu[i*4], texCoord, size);
        util::affixPos(&m_pauseMenu[i*4], pos, size, 0);
    }

    //Reading settings menu objects
    m_settingsMenu = sf::VertexArray(sf::Quads, num_s*4);
    for (unsigned i = 0; i < num_s && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_settingsMenu[i*4], texCoord, size);
        util::affixPos(&m_settingsMenu[i*4], pos, size, 0);
    }

    //Reading main menu objects, initializing appearance
    m_mainMenu = sf::VertexArray(sf::Quads, num_m*4);
    for (unsigned i = 0; i < num_m && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_mainMenu[i*4], texCoord, size);
        util::affixPos(&m_mainMenu[i*4], pos, size, 0);
    }

    //Reading level end sequence text positions
    m_scoring_numbers = std::vector<sf::Text>(6, sf::Text());
    for (unsigned i = 0; i < num_e && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        m_scoring_numbers[i].setPosition(pos);
        m_scoring_numbers[i].setFont(m_stencil);
        if (i+1 < num_e) {
            m_scoring_numbers[i].setColor(sf::Color::White);
        }
        else {
            m_scoring_numbers[i].setColor(sf::Color::Green);
            m_scoring_numbers[i].setCharacterSize(40);
        }
    }
    //texCoord doesn't matter because it will be copied from levelIcons
    util::read3v2f(fin, texCoord, size, pos, true);
    util::affixPos(m_scoringLevelDisplay, pos, size, 0);
    
    //Reading select level objects
    m_select = std::vector<sf::Vertex>(num_l*4, sf::Vertex());
    m_selectLit = std::vector<sf::Vertex>(2*4, sf::Vertex());
    m_selectDim = std::vector<sf::Vertex>(2*4, sf::Vertex());
    for (unsigned i = 0; i < 2 && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_select[i*4], texCoord, size);
        util::affixPos(&m_select[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < 2 && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_selectLit[i*4], texCoord, size);
        util::affixPos(&m_selectLit[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < 2 && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_selectDim[i*4], texCoord, size);
        util::affixPos(&m_selectDim[i*4], pos, size, 0);
    }

    fin.close();

    //Initializing appearance and configuration of objects
    m_guisheet.setSmooth(true);
    m_main.setTexture(m_mainTex);
    m_settings.setTexture(m_settingsTex);
    m_scoringScreen.setTexture(m_scoringScreenTex);
    m_main_choice = 1;
    m_settings_choice = 1;
    m_pause_choice = 1;
    m_select_choice = 1;
    for (unsigned i = 1; i < num_p; i += 2)
        util::setAlpha(&m_pauseMenu[i*4], 0);
    for (unsigned i = 1; i < num_m; i += 2)
        util::setAlpha(&m_mainMenu[i*4], 0);
    m_main_blink = 0;
    m_main_blinkChg = 5;
    m_main_blinkLoc = m_mainMenu[0].position;
    m_main_blinkSize = m_mainMenu[2].position - m_main_blinkLoc;
    m_mainInfo.setFont(m_liberation);
    m_mainInfo.setPosition(25, 525);
    m_mainInfo.setCharacterSize(25);
    m_mainInfo.setColor(sf::Color(30, 16, 8));
    util::copySprite(&m_levelIcons[0][0], m_levelDisplay);
    util::copySprite(&m_levelIcons[0][0], m_nextLevelDisplay);
    util::copyTexture(&m_levelIcons[0][0], &m_select[1*4]);
    util::copySprite(&m_selectDim[0*4], &m_select[2*4]);
    util::copySprite(&m_selectDim[1*4], &m_select[3*4]);

    return true;
}

void GameGUI::startLevelEndSequence(const std::map<std::string, int> levelEndStats)
{
    std::string dataList[] = {"accuracy", "time", "", "", "", "bonus"};
    for (unsigned i = 0; i < 6; i++) {
        std::wstringstream wss;
        wss << levelEndStats.find(dataList[i])->second;
        m_scoring_numbers[i].setString(wss.str());
    }
    int lvl = config.getInt("level");
    unsigned idx = (unsigned)(lvl-1);
    unsigned nextIdx = idx + (lvl < config.getInt("num_levels") ? 1 : 0);
    util::copyTexture(&m_levelIcons[idx][0], m_scoringLevelDisplay);
    util::copyTexture(&m_levelIcons[nextIdx][0], m_nextLevelDisplay);
    util::setAlpha(m_nextLevelDisplay, 0);
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
                mAgent->getGameState().score += mAgent->getGameState().level_bonus;
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
            util::setAlpha(m_levelDisplay, 255);
        }
        else {
            float alphaPerc = 1.f - m_levelEndSequence_timing / 1.5f; 
            alphaPerc *= alphaPerc*10*alphaPerc;
            unsigned char alpha = (unsigned char)(alphaPerc * 255);
            util::setAlpha(m_nextLevelDisplay, alpha);
            util::setAlpha(m_levelDisplay, 255 - alpha);
            gAgent->setNextLevelBGOpacity(alpha);
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
    unsigned idx = (unsigned)(config.getInt("level")-1);
    util::copyTexture(&m_levelIcons[idx][0], m_levelDisplay);

    //Updating grenade display
    m_grenadeDisplay = std::vector<sf::Vertex>();
    for (int i = 0; i < state.player->getNumGrenades(); i++) {
        sf::Vertex grenadeSprite[4];
        util::copySprite(m_grenade, grenadeSprite);
        util::translateSprite(grenadeSprite, sf::Vector2f(-25.f * i, 0));
        m_grenadeDisplay.insert(m_grenadeDisplay.end(), grenadeSprite, grenadeSprite+4);    
    }
}

void GameGUI::updateGameState(const GameState& state)
{
    GameGUI::updateHUD(state);
    if (getAppState() == LEVELENDSEQUENCE) {
        GameGUI::updateLevelEndSequence(state);
    }
}

void GameGUI::selectPauseChoice(unsigned choice)
{
    if (choice == 0) choice += m_pause_numChoices;
    else if (choice > m_pause_numChoices) choice -= m_pause_numChoices;
    util::setAlpha(&m_pauseMenu[m_pause_choice*8-4], 0);
    util::setAlpha(&m_pauseMenu[m_pause_choice*8], 255);
    util::setAlpha(&m_pauseMenu[choice*8-4], 255);
    util::setAlpha(&m_pauseMenu[choice*8], 0);
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
    util::setAlpha(&m_settingsMenu[m_settings_choice*16-8], 0);
    util::setAlpha(&m_settingsMenu[m_settings_choice*16-4], 255);
    util::setAlpha(&m_settingsMenu[choice*16-8], 255);
    util::setAlpha(&m_settingsMenu[choice*16-4], 0);
    m_settings_choice = choice;
}

void GameGUI::processSettingsChoice()
{
    if (m_settings_choice == 1) {
        config.setInt("hitbox_enabled", !config.getInt("hitbox_enabled"));
    }
    processSettingsSwitches();
}

void GameGUI::processSettingsSwitches()
{
    util::setAlpha(&m_settingsMenu[0], config.getInt("hitbox_enabled") ? 0 : 255);
    util::setAlpha(&m_settingsMenu[4], config.getInt("hitbox_enabled") ? 255 : 0);
}

void GameGUI::selectMainChoice(unsigned choice)
{
    if (choice == 0) choice += m_main_numChoices;
    else if (choice > m_main_numChoices) choice -= m_main_numChoices;
    util::setAlpha(&m_mainMenu[m_main_choice*8-4], 0);
    util::setAlpha(&m_mainMenu[m_main_choice*8], 255);
    util::setAlpha(&m_mainMenu[choice*8-4], 255);
    util::setAlpha(&m_mainMenu[choice*8], 0);
    util::affixPos(&m_mainMenu[0], m_main_blinkLoc + sf::Vector2f(0, (choice-1) * 50.f), m_main_blinkSize, 1);
    m_main_choice = choice;
}

void GameGUI::processMainChoice()
{
    if (m_main_choice == 1) startGame();
    else if (m_main_choice == 2) goToSelectLevel();
    else if (m_main_choice == 3) goToSettings();
    else if (m_main_choice == 5) endGame();
}

void GameGUI::mainBlink()
{
    if (m_main_blink + m_main_blinkChg < 0 ||
        m_main_blink + m_main_blinkChg > 255) m_main_blinkChg *= -1;
    m_main_blink += m_main_blinkChg;
    util::setAlpha(&m_mainMenu[0], (unsigned char)m_main_blink);
}

void GameGUI::selectSelectChoice(unsigned choice)
{
    if (choice == 0 || choice > (unsigned)config.getInt("num_levels")) return;
    m_select_choice = choice;
    util::copyTexture(&m_levelIcons[choice-1][0], &m_select[1*4]);
}

void GameGUI::processSelectChoice()
{
    config.setInt("level", (int)m_select_choice);
    gAgent->setNextLevelBGOpacity(0);
    goToMain();
}

void GameGUI::updateAppState()
{
    if (getAppState() == MAIN || getAppState() == SELECTLEVEL) {
        mainBlink();
        std::wstringstream wss;
        wss << "Level " << config.getInt("level") << "\nHighscore " << config.getInt("highscore");
        m_mainInfo.setString(wss.str());
        if (getAppState() == SELECTLEVEL) {
            if (config.pressing(Config::K_UP)) util::copyTexture(&m_selectLit[0*4], &m_select[2*4]);
            else util::copyTexture(&m_selectDim[0*4], &m_select[2*4]);
            if (config.pressing(Config::K_DOWN)) util::copyTexture(&m_selectLit[1*4], &m_select[3*4]);
            else util::copyTexture(&m_selectDim[1*4], &m_select[3*4]);
        }
    }
}

void GameGUI::processInput(const std::vector<sf::Event> &keyEvents)
{
    for (unsigned i = 0; i < keyEvents.size(); i++) {

        bool up = config.pressing(Config::K_UP, keyEvents[i].key.code);
        bool down = config.pressing(Config::K_DOWN, keyEvents[i].key.code);
        bool enter = keyEvents[i].key.code == sf::Keyboard::Return;
        bool esc = keyEvents[i].key.code == sf::Keyboard::Escape;

        if (getAppState() == LEVELENDSEQUENCE) {
            if (enter) forwardLevelEndSequence();
        }
        else if (getAppState() == SELECTLEVEL) {
            if (down) selectSelectChoice(m_select_choice - 1);
            else if (up) selectSelectChoice(m_select_choice + 1);
            else if (enter) processSelectChoice();
        }
        else if (getAppState() == PAUSED) {
            if (down) selectPauseChoice(m_pause_choice + 1);
            else if (up) selectPauseChoice(m_pause_choice - 1);
            else if (enter) processPauseChoice();
        }
        else if (getAppState() == SETTINGS) {
            if (down) selectSettingsChoice(m_settings_choice + 1);
            else if (up) selectSettingsChoice(m_settings_choice - 1);
            else if (enter) processSettingsChoice();
        }
        else if (getAppState() == MAIN) {
            if (down) selectMainChoice(m_main_choice + 1);
            else if (up) selectMainChoice(m_main_choice - 1);
            else if (enter) processMainChoice();
        }
        if (esc) {
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
    if (getAppState() == SELECTLEVEL) {
        selectSelectChoice((unsigned)config.getInt("level"));
    }
}

void GameGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::RenderStates s(&m_guisheet);
    if (getAppState() == MAIN || getAppState() == SELECTLEVEL) {
        target.draw(m_main);
        target.draw(m_mainMenu, &m_guisheet);
        target.draw(m_mainInfo);
        if (getAppState() == SELECTLEVEL) {
            sf::RectangleShape fade;
            fade.setFillColor(sf::Color(0, 0, 0, 100));
            fade.setPosition(0, 0);
            fade.setSize(sf::Vector2f((float)APP_WIDTH, (float)APP_HEIGHT));
            target.draw(fade);
            target.draw(&m_select[0], m_select.size(), sf::Quads, s);
        }
    }
    else if (getAppState() == SETTINGS) {
        target.draw(m_settings);
        target.draw(m_settingsMenu, &m_guisheet);
    }
    else {
        //Draw all relevant appStates
        for (unsigned i = 0; i < appStates.size(); i++) {
            if (appStates[i] == GAME) {
                target.draw(m_score);
                target.draw(m_hud, s);
                if (!m_grenadeDisplay.empty())
                    target.draw(&m_grenadeDisplay[0], m_grenadeDisplay.size(), sf::Quads, s);
            }
            else if (appStates[i] == LEVELENDSEQUENCE) {
                if (m_levelEndSequence_timing_stage == 1 || m_levelEndSequence_timing_stage == 2) {
                    target.draw(m_scoringScreen);
                    target.draw(m_scoringLevelDisplay, 4, sf::Quads, s);
                    for (int j = 0; j < m_scoring_timing_stage; j++) {
                        target.draw(m_scoring_numbers[(unsigned)j]);
                    }
                }
                else if (m_levelEndSequence_timing_stage >= 4) {
                    target.draw(m_nextLevelDisplay, 4, sf::Quads, s);
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
