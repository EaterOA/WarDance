#include "GameGUI.hpp"
#include "Util.hpp"
#include "GameConfig.hpp"
#include "GameResourceManager.hpp"
#include "GameController.hpp"
#include "App.hpp"
#include <math.h>

bool GameGUI::init(GameMechanics* m, GameGraphics* g)
{
    mAgent = m;
    gAgent = g;

    //Initializing object counts
    unsigned num_level_icons = 10,
             num_status_icons = 3,
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
    util::affixTexture(m_grenadeBase, texCoord, size);
    util::affixPos(m_grenadeBase, pos, size, 0);
    //- Bars
    m_hud = sf::VertexArray(sf::Quads, 5*4);
    for (unsigned i = 0; i < 4 && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_hud[i*4], texCoord, size);
        util::affixPos(&m_hud[i*4], pos, size, 0);
    }
    //- Statuses
    Player::StatusType statusList[] = {Player::HASTE, Player::SLOW, Player::CONFUSION};
    for (unsigned i = 0; i < num_status_icons && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        std::vector<sf::Vertex> img(4);
        util::affixTexture(&img[0], texCoord, size);
        util::affixPos(&img[0], pos, size, 0);
        m_statusIcons[statusList[i]] = img;
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

    fin.close();

    //Initializing appearance and configuration of objects
    m_scoringScreen.setTexture(resource.getTexture("scoring"));
    util::copySprite(&m_levelIcons[0][0], m_levelDisplay);
    util::copySprite(&m_levelIcons[0][0], m_nextLevelDisplay);

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
        util::copySprite(m_grenadeBase, grenadeSprite);
        util::translateSprite(grenadeSprite, sf::Vector2f(-25.f * i, 0));
        m_grenadeDisplay.insert(m_grenadeDisplay.end(), grenadeSprite, grenadeSprite+4);    
    }

    //Updating status display
    m_statusDisplay = std::vector<sf::Vertex>();
    std::list<Player::Status> sList = state.player->getStatuses();
    float offset = 0;
    for (std::list<Player::Status>::iterator iter = sList.begin(); iter != sList.end(); iter++) {
        sf::Vertex icon[4];
        util::copySprite(&m_statusIcons[iter->type][0], icon);
        util::translateSprite(icon, sf::Vector2f(offset, 0));
        unsigned char alpha = 255;
        if (iter->dur < 5.f)
            alpha = (unsigned char)(fmod(iter->dur, 1.f) * 255);
        util::setAlpha(icon, alpha);
        m_statusDisplay.insert(m_statusDisplay.end(), icon, icon+4);
        offset += 30;
    }

}

void GameGUI::updateGameState(const GameState& state)
{
    GameGUI::updateHUD(state);
    if (getAppState() == LEVELENDSEQUENCE) {
        GameGUI::updateLevelEndSequence(state);
    }
}

void GameGUI::updateAppState(const sf::Time &elapsed)
{
}

unsigned GameGUI::translateOption(float x, float y)
{
    return 0;
}

void GameGUI::processInput(const std::vector<sf::Event> &events)
{
    AppState s = getAppState();
    for (unsigned i = 0; i < events.size(); i++) {
        //Keyboard events
        if (events[i].type == sf::Event::KeyPressed) {
            bool up = controller.pressing(GameController::K_UP, events[i].key.code);
            bool down = controller.pressing(GameController::K_DOWN, events[i].key.code);
            bool enter = controller.pressing(GameController::K_ENTER, events[i].key.code);
            bool esc = controller.pressing(GameController::K_ESCAPE, events[i].key.code);

            if (s == LEVELENDSEQUENCE) {
                if (enter) forwardLevelEndSequence();
            }
            if (esc) {
                if (s == GAME || s == LEVELENDSEQUENCE) pauseGame();
            }
        }
    }
}

void GameGUI::transitionAppState()
{
}

void GameGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::RenderStates s(&resource.getTexture("guisheet"));
    //Draw all relevant appStates
    for (unsigned i = 0; i < appStates.size(); i++) {
        if (appStates[i] == GAME) {
            target.draw(m_score);
            target.draw(m_hud, s);
            if (!m_grenadeDisplay.empty())
                target.draw(&m_grenadeDisplay[0], m_grenadeDisplay.size(), sf::Quads, s);
            if (!m_statusDisplay.empty())
                target.draw(&m_statusDisplay[0], m_statusDisplay.size(), sf::Quads, s);
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
    }
}
