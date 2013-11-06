#include <vector>
#include "GameMechanics.hpp"
#include "GameGraphics.hpp"
#include "GameGUI.hpp"
#include "Util.hpp"

enum AppState {
	SPLASH, MENU, GAME, PAUSED, DEAD
};

struct AppSavedState {
	sf::Time pausedTime;
};

AppState appState;
AppSavedState savedState;
sf::RenderWindow window;
sf::View camera(sf::FloatRect(0, 0, 800, 600)), hud(sf::FloatRect(0, 0, 800, 600));
GameMechanics mAgent;
GameGraphics gAgent;
GameGUI guiAgent;

bool appInit()
{
	appState = GAME;
	srand((unsigned)time(0));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(800, 600), "War Dance", sf::Style::Close, settings);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setView(camera);
	mAgent.gameInit();
	try {
		guiAgent.init();
	}
	catch (const char* msg) {
		std::cerr << "Error: " << msg << "\n";
	}
	if (!gAgent.loadTextures()) return false;
	return true;
}

void updateView(sf::Vector2f pos)
{
	if (pos.x < 400) pos.x = 400;
	else if (pos.x > Util::MAP_WIDTH - 400) pos.x = Util::MAP_WIDTH - 400;
	if (pos.y < 300) pos.y = 300;
	else if (pos.y > Util::MAP_HEIGHT - 300) pos.y = Util::MAP_HEIGHT - 300;
	camera.setCenter(pos);
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
			if (event.type == sf::Event::LostFocus) {
				appState = PAUSED;
				savedState.pausedTime = elapsed;
			}
			if (event.type == sf::Event::GainedFocus) {
				appState = GAME;
				elapsed = savedState.pausedTime;
				clock.restart();
			}
			eventList.push_back(event);
		}
		
		if (appState == GAME) {
			mAgent.processEvents(eventList);
			mAgent.updateState(window, elapsed);
			mAgent.tick();
			bool playerDead = mAgent.cleanUp();
			gAgent.updateSprites(mAgent.getState());
			guiAgent.updateDisplay(mAgent.getState());
			updateView(mAgent.getState().player->getPos());
			window.setView(camera);
			window.draw(gAgent);
			window.setView(hud);
			window.draw(guiAgent);
			window.setView(camera);
			window.display();

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