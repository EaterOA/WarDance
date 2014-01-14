#include "App.hpp"
#include "Config.hpp"
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
AppState prevState, appState, haltState;

bool appInit()
{
	srand(unsigned(time(0)));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "War Dance", sf::Style::Close, settings);
	window.setFramerateLimit(60);
	window.setView(camera);
    conf::init_config("config/config.txt");
	if (!mAgent.init()) return false;
	if (!guiAgent.init()) return false;
	if (!gAgent.init()) return false;

	appState = MAIN;
    prevState = NONE;
    haltState = NONE;

	return true;
}

void updateView(sf::Vector2f pos)
{
	if (pos.x < APP_WIDTH/2) pos.x = APP_WIDTH/2;
	else if (pos.x > mAgent.getState().map_width - APP_WIDTH/2) pos.x = (float)mAgent.getState().map_width - APP_WIDTH/2;
	if (pos.y < 300) pos.y = 300;
	else if (pos.y > mAgent.getState().map_height - APP_HEIGHT/2) pos.y = (float)mAgent.getState().map_height - APP_HEIGHT/2;
	camera.setCenter(pos);
}

void paint()
{
	window.setView(camera);
	if (appState == PAUSED || appState == GAME) window.draw(gAgent);
	window.setView(hud);
	window.draw(guiAgent);
	window.setView(camera);
	window.display();
}

void goToMain()
{
	prevState = appState;
    appState = MAIN;
    guiAgent.transitionState(mAgent.getState());
}

void startGame()
{
    appState = GAME;
	gameClock.restart();
	mAgent.initLevel();
    guiAgent.transitionState(mAgent.getState());
    goToLevelStart();
}

void endGame()
{
	appState = CLOSED;
	window.close();
}

void resumeGame()
{
	appState = GAME;
    guiAgent.transitionState(mAgent.getState());
	gameClock.restart();
}

void pauseGame()
{
	gAgent.updateSprites(mAgent.getState());
	appState = PAUSED;
    guiAgent.transitionState(mAgent.getState());
}

void goToSettings()
{
	prevState = appState;
	appState = SETTINGS;
    guiAgent.transitionState(mAgent.getState());
}

void goToLevelEnd()
{
    prevState = appState;
    appState = LEVELEND;
    guiAgent.transitionState(mAgent.getState());
}

void goToLevelStart()
{
    mAgent.startLevel();
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
            if (haltState != NONE) continue;
			haltState = appState;
			appState = NOFOCUS;
		}
		else if (event.type == sf::Event::GainedFocus) {
            if (haltState == NONE) continue;
			if (haltState == GAME) pauseGame();
			else appState = haltState;
            haltState = NONE;
		}
		else if (event.type == sf::Event::KeyPressed) keyEvents.push_back(event);
	}
	return keyEvents;
}

void appStart()
{
	guiAgent.transitionState(mAgent.getState());
	gameClock.restart();
	while (window.isOpen()) {
		std::vector<sf::Event> keyEvents = processEvents();
		guiAgent.updateAppState(keyEvents);
		if (appState == GAME || appState == LEVELEND) {
			mAgent.updateState(window, gameClock.restart());
			int gameStatus = mAgent.tick();
			gAgent.updateSprites(mAgent.getState());
			guiAgent.updateGameState(mAgent.getState());
			updateView(mAgent.getState().player->getPos());
			if (gameStatus == 1) {
				goToMain();
			}
            else if (gameStatus == 2 && appState == GAME) {
                //goToLevelEnd();
                mAgent.endLevel();
                if (config["level"] < config["num_levels"])
                    config["level"]++;
                mAgent.startLevel();
            }
		}
        if (appState != NOFOCUS) paint();
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
