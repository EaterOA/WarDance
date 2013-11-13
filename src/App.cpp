#include "App.hpp"
#include "GameMechanics.hpp"
#include "GameGraphics.hpp"
#include "GameGUI.hpp"
#include <vector>

struct AppSavedState {
	sf::Time pausedTime;
};

AppState appState;
AppSavedState savedState;
sf::RenderWindow window;
sf::View camera(sf::FloatRect(0, 0, (float)APP_WIDTH, (float)APP_HEIGHT));
sf::View hud(sf::FloatRect(0, 0, (float)APP_WIDTH, (float)APP_HEIGHT));
GameMechanics mAgent;
GameGraphics gAgent;
GameGUI guiAgent;

bool appInit()
{
	appState = GAME;
	srand((unsigned)time(0));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "War Dance", sf::Style::Close, settings);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	window.setView(camera);
	mAgent.gameInit();
	if (!guiAgent.init()) return false;
	if (!gAgent.init()) return false;
	return true;
}

void updateView(sf::Vector2f pos)
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

void paint()
{
	window.setView(camera);
	window.draw(gAgent);
	window.setView(hud);
	window.draw(guiAgent);
	window.setView(camera);
	window.display();
}

void appStart()
{
	sf::Clock clock;
	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();
		sf::Event event;
		std::vector<sf::Event> eventList;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
			else if (event.type == sf::Event::LostFocus) {
				appState = NOFOCUS;
				savedState.pausedTime = elapsed;
			}
			else if (event.type == sf::Event::GainedFocus) {
				appState = PAUSED;
				guiAgent.transitionState(appState);
				elapsed = savedState.pausedTime;
				clock.restart();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				if (appState == PAUSED) appState = GAME;
				else appState = PAUSED;
				guiAgent.transitionState(appState);
			}
			else eventList.push_back(event);
		}
		
		if (appState == PAUSED) {
			paint();
		}
		if (appState == GAME) {
			mAgent.processEvents(eventList);
			mAgent.updateState(window, elapsed);
			mAgent.tick();
			bool playerDead = mAgent.cleanUp();
			gAgent.updateSprites(mAgent.getState());
			guiAgent.updateDisplay(mAgent.getState());
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