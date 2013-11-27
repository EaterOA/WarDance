#include "App.hpp"
#include "GameMechanics.hpp"
#include "GameGraphics.hpp"
#include "GameGUI.hpp"
#include <vector>

sf::RenderWindow window;
sf::View camera(sf::FloatRect(0, 0, float(APP_WIDTH), float(APP_HEIGHT)));
sf::View hud(sf::FloatRect(0, 0, float(APP_WIDTH), float(APP_HEIGHT)));
GameMechanics mAgent;
GameGraphics gAgent;
GameGUI guiAgent;
sf::Clock gameClock;
AppState appState;

static bool appInit()
{
	appState = GAME;
	srand(unsigned(time(0)));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "War Dance", sf::Style::Close, settings);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);
	window.setView(camera);
	mAgent.gameInit();
	if (!guiAgent.init()) return false;
	if (!gAgent.init()) return false;
	return true;
}

static void updateView(sf::Vector2f pos)
{
	if (pos.x < APP_WIDTH/2)
		pos.x = APP_WIDTH/2;
	else if (pos.x > mAgent.getState().map_width - APP_WIDTH/2)
		pos.x = (float)mAgent.getState().map_width - APP_WIDTH/2;
	if (pos.y < 300)
		pos.y = 300;
	else if (pos.y > mAgent.getState().map_height - APP_HEIGHT/2)
		pos.y = (float)mAgent.getState().map_height - APP_HEIGHT/2;
	camera.setCenter(pos);
}

static void paint()
{
	window.setView(camera);
	window.draw(gAgent);
	window.setView(hud);
	window.draw(guiAgent);
	window.setView(camera);
	window.display();
}

void endGame()
{
	appState = CLOSED;
	window.close();
}

void resumeGame()
{
	AppState prev = appState;
	appState = GAME;
	guiAgent.transitionState(prev);
	gameClock.restart();
}

void pauseGame()
{
	AppState prev = appState;
	appState = PAUSED;
	guiAgent.transitionState(prev);
}

static std::vector<sf::Event> processEvents()
{
	std::vector<sf::Event> keyEvents;
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			endGame();
			break;
		}
		else if (event.type == sf::Event::LostFocus) {
			appState = NOFOCUS;
		}
		else if (event.type == sf::Event::GainedFocus) {
			pauseGame();
		}
		else if (event.type == sf::Event::KeyPressed) keyEvents.push_back(event);
	}
	return keyEvents;
}

static void appStart()
{
	gameClock.restart();
	while (window.isOpen()) {

		std::vector<sf::Event> keyEvents = processEvents();
		
		guiAgent.updateAppState(keyEvents);

		if (appState == PAUSED) {
			paint();
		}
		else if (appState == GAME) {
			mAgent.updateState(window, gameClock.restart());
			mAgent.tick();
			bool playerDead = mAgent.cleanUp();
			gAgent.updateSprites(mAgent.getState());
			guiAgent.updateGameState(mAgent.getState());
			updateView(mAgent.getState().player->getPos());
			paint();
			if (playerDead) {
				window.close();
				return;
			}
		}
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
