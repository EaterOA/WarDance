#include "GameGUI.hpp"
#include "Util.hpp"
#include "GameConfig.hpp"
#include "GameResourceManager.hpp"
#include "GameController.hpp"
#include "App.hpp"

bool GameGUI::init(GameMechanics* m, GameGraphics* g)
{
    mAgent = m;
    gAgent = g;

    //Initializing object counts
    m_pause_numChoices = 3;
    m_settings_numChoices = 1;
    m_main_numChoices = 5;
    unsigned num_level_icons = 10,
             num_p = 1,
             num_s = 0,
             num_m = 1,
             num_l = 2,
             num_ll = 2,
             num_e = 6;

    //Preparing to load gui object data
    //Data format: 2 base texture coordinates, 2 texture size, 2 (center) position coordinates
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
    m_score.setFont(resource.getFont("stencil"));
    m_score.setPosition(22, 560);
    m_score.setCharacterSize(24);

    //Reading pause menu objects, initializing appearance
    m_pause = std::vector<sf::Vertex>((num_p + m_pause_numChoices)*4, sf::Vertex());
    m_pauseLit = m_pauseDim = std::vector<sf::Vertex>(m_pause_numChoices*4, sf::Vertex());
    for (unsigned i = 0; i < num_p && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_pause[i*4], texCoord, size);
        util::affixPos(&m_pause[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < m_pause_numChoices && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_pauseLit[i*4], texCoord, size);
        util::affixPos(&m_pauseLit[i*4], pos, size, 0);
        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_pauseDim[i*4], texCoord, size);
        util::affixPos(&m_pauseDim[i*4], pos, size, 0);
    }
    m_pauseMenu = &m_pause[num_p*4];

    //Reading settings menu objects
    m_settings = std::vector<sf::Vertex>((num_s + m_settings_numChoices*2)*4, sf::Vertex());
    m_settingsLit = m_settingsDim = std::vector<sf::Vertex>(m_settings_numChoices*4, sf::Vertex());
    m_settingsOn = m_settingsOff = std::vector<sf::Vertex>(m_settings_numChoices*4, sf::Vertex());
    for (unsigned i = 0; i < num_s && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_settings[i*4], texCoord, size);
        util::affixPos(&m_settings[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < m_settings_numChoices && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_settingsLit[i*4], texCoord, size);
        util::affixPos(&m_settingsLit[i*4], pos, size, 0);

        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_settingsDim[i*4], texCoord, size);
        util::affixPos(&m_settingsDim[i*4], pos, size, 0);

        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_settingsOn[i*4], texCoord, size);
        util::affixPos(&m_settingsOn[i*4], pos, size, 0);

        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_settingsOff[i*4], texCoord, size);
        util::affixPos(&m_settingsOff[i*4], pos, size, 0);
    }
    m_settingsMenu = &m_settings[num_s*4];
    m_settingsSwitches = &m_settings[(num_s+m_settings_numChoices)*4];

    //Reading main menu objects, initializing appearance
    m_main = std::vector<sf::Vertex>((num_m + m_main_numChoices)*4, sf::Vertex());
    m_mainLit = m_mainDim = std::vector<sf::Vertex>(m_main_numChoices*4, sf::Vertex());
    for (unsigned i = 0; i < num_m && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_main[i*4], texCoord, size);
        util::affixPos(&m_main[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < m_main_numChoices && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_mainLit[i*4], texCoord, size);
        util::affixPos(&m_mainLit[i*4], pos, size, 0);
        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_mainDim[i*4], texCoord, size);
        util::affixPos(&m_mainDim[i*4], pos, size, 0);
    }
    m_main_blinker = &m_main[0];
    m_mainMenu = &m_main[num_m*4];

    //Reading level end sequence text positions
    m_scoring_numbers = std::vector<sf::Text>(6, sf::Text());
    for (unsigned i = 0; i < num_e && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        m_scoring_numbers[i].setPosition(pos);
        m_scoring_numbers[i].setFont(resource.getFont("stencil"));
        if (i+1 < num_e) {
            m_scoring_numbers[i].setColor(sf::Color::White);
        }
        else {
            m_scoring_numbers[i].setColor(sf::Color::Green);
            m_scoring_numbers[i].setCharacterSize(40);
        }
    }
    util::read3v2f(fin, texCoord, size, pos, true);
    util::affixPos(m_scoringLevelDisplay, pos, size, 0);
    
    //Reading select level objects
    m_select = std::vector<sf::Vertex>((num_l + num_ll)*4, sf::Vertex());
    m_selectLit = m_selectDim = std::vector<sf::Vertex>(num_ll*4, sf::Vertex());
    for (unsigned i = 0; i < num_l && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_select[i*4], texCoord, size);
        util::affixPos(&m_select[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < num_ll && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_selectLit[i*4], texCoord, size);
        util::affixPos(&m_selectLit[i*4], pos, size, 0);
        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_selectDim[i*4], texCoord, size);
        util::affixPos(&m_selectDim[i*4], pos, size, 0);
    }

    fin.close();

    //Initializing appearance and configuration of objects
    m_main_bg.setTexture(resource.getTexture("main_bg"));
    m_settings_bg.setTexture(resource.getTexture("settings_bg"));
    m_scoringScreen.setTexture(resource.getTexture("scoring"));
    m_main_choice = 1;
    m_settings_choice = 1;
    m_pause_choice = 1;
    m_select_choice = 1;
    m_main_blinkerBaseLoc = m_main_blinker[0].position;
    m_main_blinkerAlphaChg = 5;
    m_mainInfo.setFont(resource.getFont("liberation"));
    m_mainInfo.setPosition(25, 525);
    m_mainInfo.setCharacterSize(25);
    m_mainInfo.setColor(sf::Color(30, 16, 8));
    m_select_upLitTime = m_select_downLitTime = 0.f;
    util::copySprite(&m_levelIcons[0][0], m_levelDisplay);
    util::copySprite(&m_levelIcons[0][0], m_nextLevelDisplay);
    util::copyTexture(&m_levelIcons[0][0], &m_select[1*4]);
    for (unsigned i = 0; i < m_pause_numChoices; i++) util::copySprite(&m_pauseDim[i*4], &m_pause[(num_p+i)*4]);
    for (unsigned i = 0; i < m_settings_numChoices; i++) util::copySprite(&m_settingsDim[i*4], &m_settings[(num_s+i)*4]);
    for (unsigned i = 0; i < m_main_numChoices; i++) util::copySprite(&m_mainDim[i*4], &m_main[(num_m+i)*4]);
    for (unsigned i = 0; i < num_ll; i++) util::copySprite(&m_selectDim[i*4], &m_select[(num_l+i)*4]);

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

    m_statusDisplay = std::vector<sf::Vertex>();

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
    unsigned prev = m_pause_choice-1;
    unsigned next = choice-1;
    util::copyTexture(&m_pauseDim[prev*4], &m_pauseMenu[prev*4]);
    util::copyTexture(&m_pauseLit[next*4], &m_pauseMenu[next*4]);
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
    unsigned prev = m_settings_choice-1;
    unsigned next = choice-1;
    util::copyTexture(&m_settingsDim[prev*4], &m_settingsMenu[prev*4]);
    util::copyTexture(&m_settingsLit[next*4], &m_settingsMenu[next*4]);
    m_settings_choice = choice;
}

void GameGUI::processSettingsChoice()
{
    if (m_settings_choice == 1) {
        config.setInt("hitbox_enabled", !config.getInt("hitbox_enabled"));
    }
}

void GameGUI::processSettingsSwitches()
{
    util::copySprite((config.getInt("hitbox_enabled") ? &m_settingsOn[0] : &m_settingsOff[0]), &m_settingsSwitches[0]);
}

void GameGUI::selectMainChoice(unsigned choice)
{
    if (choice == 0) choice += m_main_numChoices;
    else if (choice > m_main_numChoices) choice -= m_main_numChoices;
    unsigned prev = m_main_choice-1;
    unsigned next = choice-1;
    util::copySprite(&m_mainDim[prev*4], &m_mainMenu[prev*4]);
    util::copySprite(&m_mainLit[next*4], &m_mainMenu[next*4]);
    m_main_choice = choice;
}

void GameGUI::processMainChoice()
{
    if (m_main_choice == 1) startGame();
    else if (m_main_choice == 2) goToSelectLevel();
    else if (m_main_choice == 3) goToSettings();
    else if (m_main_choice == 5) endGame();
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

void GameGUI::updateAppState(const sf::Time &elapsed)
{
    if (getAppState() == SETTINGS) {
        processSettingsSwitches();
    }

    if (getAppState() == MAIN || getAppState() == SELECTLEVEL) {

        //Blinking main menu highlight
        int a = (int)m_main_blinker[0].color.a;
        sf::Vector2f main_blinkerSize = m_main_blinker[2].position - m_main_blinker[0].position;
        sf::Vector2f main_blinkerLoc = m_main_blinkerBaseLoc + sf::Vector2f(0, (m_main_choice-1) * 50.f);
        util::affixPos(&m_main_blinker[0], main_blinkerLoc, main_blinkerSize, 1);
        if (a + m_main_blinkerAlphaChg < 0 || a + m_main_blinkerAlphaChg > 255) m_main_blinkerAlphaChg *= -1;
        util::setAlpha(&m_main[0], (unsigned char)(a + m_main_blinkerAlphaChg));

        //Updating info
        std::wstringstream wss;
        wss << "Level " << config.getInt("level") << "\nHighscore " << config.getInt("highscore");
        m_mainInfo.setString(wss.str());

        //Lighting or dimming select level arrows
        m_select_upLitTime -= elapsed.asSeconds();
        m_select_downLitTime -= elapsed.asSeconds();
        if (m_select_upLitTime > 0) util::copyTexture(&m_selectLit[0*4], &m_select[2*4]);
        else util::copyTexture(&m_selectDim[0*4], &m_select[2*4]);
        if (m_select_downLitTime > 0) util::copyTexture(&m_selectLit[1*4], &m_select[3*4]);
        else util::copyTexture(&m_selectDim[1*4], &m_select[3*4]);
    }
}

unsigned GameGUI::translateOption(float x, float y)
{
    AppState state = getAppState();
    sf::Vector2f m(x, y);
    if (state == MAIN) {
        for (unsigned i = 0; i < m_main_numChoices; i++) {
            unsigned idx = (i+1)*4;
            sf::Vector2f size = m_main[idx+2].position - m_main[idx].position;
            sf::Vector2f pos = util::referenceToCenter(m_main[idx].position, size, 1);
            if (util::hasCollided(m, pos, size, 0)) return i+1;
        }
    }
    else if (state == PAUSED) {
        for (unsigned i = 0; i < m_pause_numChoices; i++) {
            unsigned idx = (i+1)*4;
            sf::Vector2f size = m_pause[idx+2].position - m_pause[idx].position;
            sf::Vector2f pos = util::referenceToCenter(m_pause[idx].position, size, 1);
            if (util::hasCollided(m, pos, size, 0)) return i+1;
        }
    }
    else if (state == SETTINGS) {
        for (unsigned i = 0; i < m_settings_numChoices; i++) {
            unsigned idx = i*4;
            sf::Vector2f size = m_settingsMenu[idx+2].position - m_settingsMenu[idx].position;
            sf::Vector2f pos = util::referenceToCenter(m_settingsMenu[idx].position, size, 1);
            if (util::hasCollided(m, pos, size, 0)) return i+1;
        }
    }
    else if (state == SELECTLEVEL) {
        for (unsigned i = 0; i < 2; i++) {
            unsigned idx = (i+2)*4;
            sf::Vector2f size = m_select[idx+2].position - m_select[idx].position;
            sf::Vector2f pos = util::referenceToCenter(m_select[idx].position, size, 1);
            if (util::hasCollided(m, pos, size, 0)) return i+1;
        }
    }    return 0;
}

void GameGUI::processInput(const std::vector<sf::Event> &events)
{
    AppState s = getAppState();
    for (unsigned i = 0; i < events.size(); i++) {

        //Mouse presses
        if (events[i].type == sf::Event::MouseButtonReleased) {
            if (events[i].mouseButton.button == sf::Mouse::Left) {
                float x = (float)events[i].mouseButton.x;
                float y = (float)events[i].mouseButton.y;
                unsigned opt = translateOption(x, y);
                if (!opt) continue;
                if (s == MAIN) processMainChoice();
                else if (s == PAUSED) processPauseChoice();
                else if (s == SETTINGS) processSettingsChoice();
                else if (s == SELECTLEVEL) {
                    if (opt == 1) {
                        m_select_upLitTime = 0.1f;
                        selectSelectChoice(m_select_choice + 1);
                    }
                    else if (opt == 2) {
                        m_select_downLitTime = 0.1f;
                        selectSelectChoice(m_select_choice - 1);
                    }
                }
            }
            else if (events[i].mouseButton.button == sf::Mouse::Right) {
                if (s != GAME && s != LEVELENDSEQUENCE && s != MAIN) back();
            }
        }
        //Mouse movement
        else if (events[i].type == sf::Event::MouseMoved) {
            float x = (float)events[i].mouseMove.x;
            float y = (float)events[i].mouseMove.y;
            unsigned opt = translateOption(x, y);
            if (!opt) continue;
            if (s == MAIN) selectMainChoice(opt);
            else if (s == PAUSED) selectPauseChoice(opt);
            else if (s == SETTINGS) selectSettingsChoice(opt);
        }
        //Keyboard events
        else if (events[i].type == sf::Event::KeyPressed) {
            bool up = controller.pressing(GameController::K_UP, events[i].key.code);
            bool down = controller.pressing(GameController::K_DOWN, events[i].key.code);
            bool enter = controller.pressing(GameController::K_ENTER, events[i].key.code);
            bool esc = controller.pressing(GameController::K_ESCAPE, events[i].key.code);

            if (s == LEVELENDSEQUENCE) {
                if (enter) forwardLevelEndSequence();
            }
            else if (s == SELECTLEVEL) {
                if (down) {
                    m_select_downLitTime = 0.1f;
                    selectSelectChoice(m_select_choice - 1);
                }
                else if (up) {
                    m_select_upLitTime = 0.1f;
                    selectSelectChoice(m_select_choice + 1);
                }
                else if (enter) processSelectChoice();
            }
            else if (s == PAUSED) {
                if (down) selectPauseChoice(m_pause_choice + 1);
                else if (up) selectPauseChoice(m_pause_choice - 1);
                else if (enter) processPauseChoice();
            }
            else if (s == SETTINGS) {
                if (down) selectSettingsChoice(m_settings_choice + 1);
                else if (up) selectSettingsChoice(m_settings_choice - 1);
                else if (enter) processSettingsChoice();
            }
            else if (s == MAIN) {
                if (down) selectMainChoice(m_main_choice + 1);
                else if (up) selectMainChoice(m_main_choice - 1);
                else if (enter) processMainChoice();
            }
            if (esc) {
                if (s == GAME || s == LEVELENDSEQUENCE) pauseGame();
                else if (s != MAIN) back();
            }
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
    sf::RenderStates s(&resource.getTexture("guisheet"));
    if (getAppState() == MAIN || getAppState() == SELECTLEVEL) {
        target.draw(m_main_bg);
        target.draw(&m_main[0], m_main.size(), sf::Quads, s);
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
        target.draw(m_settings_bg);
        target.draw(&m_settings[0], m_settings.size(), sf::Quads, s);
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
                target.draw(&m_pause[0], m_pause.size(), sf::Quads, s);
            }
        }
    }
}
