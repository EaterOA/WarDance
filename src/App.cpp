#include "App.hpp"
#include "GameConfig.hpp"
#include "GameMechanics.hpp"
#include "GameGraphics.hpp"
#include "GameGUI.hpp"

sf::RenderWindow window;
sf::View camera(sf::FloatRect(0, 0, float(APP_WIDTH), float(APP_HEIGHT)));
sf::View hud(sf::FloatRect(0, 0, float(APP_WIDTH), float(APP_HEIGHT)));
GameMechanics mAgent;
GameGraphics gAgent;
GameGUI guiAgent;
sf::Clock gameClock;
std::vector<AppState> appStates;

bool appInit()
{
    srand(unsigned(time(0)));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "War Dance", sf::Style::Close, settings);
    window.setFramerateLimit(60);
    window.setView(camera);
    config.loadConfig("config/config.txt");
    if (!mAgent.init()) return false;
    if (!gAgent.init()) return false;
    if (!guiAgent.init(&mAgent, &gAgent)) return false;

    appStates.push_back(MAIN);

    return true;
}

void updateView(sf::Vector2f pos)
{
    if (pos.x < APP_WIDTH/2) pos.x = APP_WIDTH/2;
    else if (pos.x > mAgent.getGameState().map_width - APP_WIDTH/2) pos.x = (float)mAgent.getGameState().map_width - APP_WIDTH/2;
    if (pos.y < 300) pos.y = 300;
    else if (pos.y > mAgent.getGameState().map_height - APP_HEIGHT/2) pos.y = (float)mAgent.getGameState().map_height - APP_HEIGHT/2;
    camera.setCenter(pos);
}

void paint()
{
    window.setView(camera);
    if (getAppState() == PAUSED || getAppState() == GAME || getAppState() == LEVELENDSEQUENCE) window.draw(gAgent);
    window.setView(hud);
    window.draw(guiAgent);
    window.setView(camera);
    window.display();
}

void back()
{
    if (!appStates.empty()) appStates.pop_back();
    if (getAppState() == GAME || getAppState() == LEVELENDSEQUENCE) {
        gameClock.restart();
    }
    guiAgent.transitionAppState();
}

AppState getAppState()
{
    if (!appStates.empty()) return appStates.back();
    return NONE;
}

void goToMain()
{
    while (appStates.back() != MAIN) back();
    guiAgent.transitionAppState();
}

void startGame()
{
    appStates.push_back(GAME);
    guiAgent.transitionAppState();
    gameClock.restart();
    mAgent.initLevel();
    mAgent.startLevel();
}

void endGame()
{
    while (!appStates.empty()) appStates.pop_back();
    appStates.push_back(CLOSED);
    window.close();
}

void pauseGame()
{
    appStates.push_back(PAUSED);
    guiAgent.transitionAppState();
}

void goToSettings()
{
    appStates.push_back(SETTINGS);
    guiAgent.transitionAppState();
}

void goToSelectLevel()
{
    appStates.push_back(SELECTLEVEL);
    guiAgent.transitionAppState();
}

void goToLevelEndSequence()
{
    appStates.push_back(LEVELENDSEQUENCE);
    guiAgent.startLevelEndSequence(mAgent.getLevelStats());
    mAgent.clearEnemyProjectiles();
}

void goToNextLevel()
{
    while (getAppState() != GAME) appStates.pop_back();
    int lvl = config.getInt("level");
    if (lvl < config.getInt("num_levels"))
        config.setInt("level", lvl+1);
    mAgent.clearPlayerProjectiles();
    mAgent.startLevel();
    guiAgent.transitionAppState();
}

std::vector<sf::Event> processEvents()
{
    std::vector<sf::Event> keyEvents;
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            endGame();
            break;
        }
        else if (event.type == sf::Event::LostFocus) {
            if (getAppState() != NOFOCUS) appStates.push_back(NOFOCUS);
        }
        else if (event.type == sf::Event::GainedFocus) {
            while (appStates.back() == NOFOCUS) appStates.pop_back();
            if (getAppState() == GAME) pauseGame();
        }
        else if (event.type == sf::Event::KeyPressed) keyEvents.push_back(event);
    }
    return keyEvents;
}

void appStart()
{
    guiAgent.transitionAppState();
    //Game loop
    while (window.isOpen()) {
        guiAgent.processInput(processEvents());
        guiAgent.updateAppState();
        if (getAppState() == GAME || getAppState() == LEVELENDSEQUENCE) {
            mAgent.updateGameState(window, gameClock.restart());
            if (mAgent.isPlayerDead()) {
                goToMain();
            }
            else if (mAgent.isLevelDone()) {
                if (!guiAgent.isLevelEndSequenceStarted()) {
                    goToLevelEndSequence();
                }
                else if (guiAgent.isLevelEndSequenceDone()) {
                    goToNextLevel();
                }
            }
            gAgent.updateMisc(mAgent.getGameState());
            guiAgent.updateGameState(mAgent.getGameState());
            gAgent.updateSprites(mAgent.getGameState());
            updateView(mAgent.getGameState().player->getPos());
        }
        if (getAppState() != NOFOCUS) paint();
    }
}

int main()
{
    if (!appInit()) {
        std::cerr << "Cannot initialize application. Aborting.\n";
        if (window.isOpen()) window.close();
    }
    else {
        appStart();
    }
}
